#include "archiving.h"

Archiving::Archiving()
{

}

void Archiving::ArchiveFile(QString filePath, QString outputFilePath)
{
    QFile *file = new QFile(filePath);
    unsigned short weights[256];
    for (int i = 0; i < 256; i++)
        weights[i] = 0;

    QTextStream input(file);
    if(!file->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to open file");
        delete file;
        return;
    }
    char buffer;
    while (!input.atEnd())
    {
        input >> buffer;
        weights[(int)buffer]++;
    }
    input.seek(0);
    std::priority_queue<TreeNode, std::vector<TreeNode>, CompareNode> symbolsQueue;
    std::list<TreeNode> symbolsList;
    for (int i = 0; i < 256; i++)
    {
        if (weights[i] > 0)
        {
            char *symbol = new char;
            *symbol = char(i);
            TreeNode *newlistItem = new TreeNode(weights[i], symbol, nullptr, nullptr, nullptr);
            symbolsList.push_front(*newlistItem);
        }
    }

    for (auto it = symbolsList.begin(); it != symbolsList.end(); it++)
        symbolsQueue.push(*it->thisNode);

    while(symbolsQueue.size() > 1)
    {
        TreeNode right = symbolsQueue.top();
        symbolsQueue.pop();
        TreeNode left = symbolsQueue.top();
        symbolsQueue.pop();
        unsigned short newKey = left.GetKey() + right.GetKey();
        TreeNode *newNode = new TreeNode(newKey, nullptr, left.thisNode, right.thisNode, nullptr);
        left.thisNode->parent = newNode->thisNode;
        right.thisNode->parent = newNode->thisNode;
        symbolsQueue.push(*newNode);
    }

    TreeNode root = symbolsQueue.top();
    symbolsQueue.pop();
    Tree *HaffmanTree = new Tree(root.thisNode);
    std::map<char, QBitArray> codeTable;
    for (auto it = symbolsList.begin(); it != symbolsList.end(); it++)
    {
        char symbol = *(it->thisNode->GetSymbol());
        QBitArray code = HaffmanTree->GetCode(it->thisNode);
        codeTable.insert(std::pair<char, QBitArray>(symbol, code));
    }

    QFile *outputFile = new QFile(outputFilePath);
    if(!outputFile->open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to create a file");
        delete outputFile;
        delete file;
        HaffmanTree->Clear();
        symbolsList.clear();
        while(!symbolsQueue.empty())
        {
            symbolsQueue.pop();
        }
        return;
    }
    QByteArray Tree;
    unsigned short lstSize = symbolsList.size();
    char byte1 = (char)(lstSize >> 8);
    Tree.append(byte1);
    char byte2 = (char)((lstSize << 8) >> 8);
    Tree.append(byte2);
    for (auto it = symbolsList.begin(); it != symbolsList.end(); it++)
    {
        char byte1 = *(it->thisNode->GetSymbol());
        Tree.append(byte1);
        unsigned short code = it->GetKey();
        unsigned short b3 = (code << 8) >> 8;
        unsigned short b2 = code >> 8;
        unsigned char cb2 = (char)b2;
        unsigned char cb3 = (char)b3;
        Tree.append(cb2);
        Tree.append(cb3);
    }
    outputFile->write(Tree);
    QBitArray stream;
    char inputChar = 0;
    while (!input.atEnd())
    {
        input >> buffer;
        QBitArray inputBitArray = codeTable[buffer];

        int j = stream.size();
        stream.resize(stream.size() + inputBitArray.size());
        for (int i = 0; i < inputBitArray.size(); i++, j++)
            stream.setBit(j, inputBitArray.at(i));
        if(stream.size() >= 8)
        {
            for (int i = 0; i < 8; i ++)
            {
                if(stream.at(i))
                    inputChar = inputChar | (1 << (7 - i));
            }
            QByteArray inputByte;
            inputByte.append(inputChar);
            outputFile->write(inputByte);
            inputByte.clear();
            inputChar = 0;
            for (int i = 8, j = 0; i < stream.size(); i++, j++)
            {
                stream.setBit(j, stream.at(i));
            }
            stream.resize(stream.size() - 8);
        }
        inputBitArray.clear();
    }
    char wasteBits = 0;
    if(stream.size() > 0)
    {
        wasteBits = 8 - stream.size();
        inputChar = 0;
        for (int i = 0; i < stream.size(); i++)
        {
            if(stream.at(i))
                inputChar = inputChar | (1 << (7 - i));
        }
        QByteArray inputByte;
        inputByte.append(inputChar);
        outputFile->write(inputByte);
        inputByte.clear();
    }
    QByteArray inputByte;
    inputByte.append(wasteBits);
    outputFile->write(inputByte);

    outputFile->close();
    delete outputFile;
    delete file;
    HaffmanTree->Clear();
    symbolsList.clear();
}

void Archiving::DecompressFile(QString inputPath, QString outputPath)
{
    QFile *inputFile = new QFile(inputPath);
    if(!inputFile->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to open file");
        delete inputFile;
        return;
    }
    QByteArray inAr =  inputFile->readAll();
    char *data = inAr.data();
    char byte1 = data[0];
    char byte2 = data[1];
    unsigned short mapSize = (int)byte1;
    mapSize = mapSize << 8;
    unsigned short tmp = (int)byte2;
    mapSize = mapSize | tmp;
    int counter = 2;
    std::priority_queue<TreeNode, std::vector<TreeNode>, CompareNode> symbolsQueue;
    for (int i = 0; i < mapSize; i++)
    {
        char *symbol = new char;
        *symbol = inAr[counter];
        unsigned char byte1;
        byte1 = inAr[counter + 1];
        unsigned char byte2;
        byte2 = inAr[counter + 2];
        unsigned short key = 0;
        key = byte1;
        key = key << 8;
        key = key | byte2;
        TreeNode *newlistItem = new TreeNode(key, symbol, nullptr, nullptr, nullptr);
        symbolsQueue.push(*newlistItem);
        counter += 3;
    }

    while(symbolsQueue.size() > 1)
    {
        TreeNode right = symbolsQueue.top();
        symbolsQueue.pop();
        TreeNode left = symbolsQueue.top();
        symbolsQueue.pop();
        unsigned short newKey = left.GetKey() + right.GetKey();
        TreeNode *newNode = new TreeNode(newKey, nullptr, left.thisNode, right.thisNode, nullptr);
        left.thisNode->parent = newNode->thisNode;
        right.thisNode->parent = newNode->thisNode;
        symbolsQueue.push(*newNode);
    }

    TreeNode root = symbolsQueue.top();
    symbolsQueue.pop();
    Tree *HaffmanTree = new Tree(root.thisNode);
    //HaffmanTree->PostOrder();
    inputFile->close();
    inputFile->open(QFile::ReadOnly);

    QByteArray inputBytes = inputFile->readAll();
    char *cdata = inputBytes.data();
    char wasteBits = cdata[inputBytes.size() - 1];
    inputBytes.resize(inputBytes.size() - 1);
    int size = 8 * (inputBytes.size() - 2 - mapSize * 3);
    QBitArray inputBits;
    inputBits.resize(size);
    counter = 0;
    for (int i = 2 + mapSize * 3; i < inputBytes.size(); i++)
    {
        char tmp = data[i];
        for (int j = 0; j < 8; j++)
        {
            bool dig = tmp & (1 << (7 - j));
            inputBits.setBit(counter, dig);
            counter++;
        }
    }
    inputBits.resize(inputBits.size() - wasteBits);
    QFile *outputFile = new QFile(outputPath);
    if(!outputFile->open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(nullptr, "Error", "Unable to open file");
        delete inputFile;
        delete outputFile;
        HaffmanTree->Clear();
        return;
    }
    HaffmanTree->Decompress(inputBits, outputFile);
    outputFile->close();
    delete inputFile;
    delete outputFile;
    HaffmanTree->Clear();
}
