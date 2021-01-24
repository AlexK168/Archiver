#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->tableView->setModel(model);
    this->setWindowIcon(QPixmap(":/img/images/winRAR.png"));
    this->setFixedSize(this->width(), this->height());
}

QString GetNewPath(QString oldPath)
{
    QFile testFile(oldPath);
    if(testFile.exists())
    {
        int closeBracket = oldPath.lastIndexOf(")");
        int openBracket = oldPath.lastIndexOf("(");
        int length = closeBracket - openBracket - 1;
        QString number = oldPath.mid(openBracket + 1, length);
        int num = number.toInt();
        num++;
        oldPath.resize(oldPath.size() - 6 - length);
        oldPath += ("(" + QString::number(num) + ").txt");
        oldPath = GetNewPath(oldPath);
    }

    return oldPath;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    QApplication::quit();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    QFileInfo fileInfo = model->fileInfo(index);
    if(fileInfo.fileName() == ".." && index.column() == 0)
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        ui->tableView->setRootIndex(model->index(dir.absolutePath()));
        QString str = model->fileInfo(index).absoluteFilePath();
        ui->lineEdit->setText(str);
    }
    else if (fileInfo.fileName() == "." && index.column() == 0)
    {
        ui->tableView->setRootIndex(model->index(""));
        ui->lineEdit->setText("");
    }
    else if(fileInfo.isDir() && index.column() == 0)
    {
        ui->tableView->setRootIndex(index);
        QString str = model->fileInfo(index).absoluteFilePath();
        ui->lineEdit->setText(str);
    }
}

void MainWindow::on_action_9_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    on_tableView_doubleClicked(index);
}

void MainWindow::on_action_12_triggered()
{
    QString info = "Курсовой проект \"Archiver 1.0\"\nАлександр Кульша\nУО БГУИР, гр.853505\n2019";
    QMessageBox::information(this, "Информация", info);
}

void MainWindow::on_action_11_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QString path = model->fileInfo(index).absoluteFilePath();
    QString fileName = model->fileInfo(index).fileName();
    QString extension = "";
    int ext = fileName.lastIndexOf(".");
    if (ext != -1 && model->fileInfo(index).isFile())
        extension = fileName.mid(ext, fileName.length() - ext);

    if(fileName == "." || fileName == ".." || index.row() == -1 || index.column() == -1 || fileName == "")
    {
        QMessageBox::warning(this, "Внимание", "Выберите папку или файл!");
        return;
    }

    fileName = fileName.mid(0,ext);
    bool cancelProc;
    QString sign = "Переименовать \"" + fileName + "\" в:";
    QString newFileName = QInputDialog::getText( this, "Ввод", sign, QLineEdit::Normal, fileName, &cancelProc);
    if (!cancelProc)
        return;
    if (newFileName == "")
    {
        QMessageBox::warning(this, "Внимание", "Введите название!");
        return;
    }
    QDir dir = model->fileInfo(index).dir();
    QString dirPath = dir.path();
    QString newPath = dirPath + "/" + newFileName + extension;
    QFile testFile(newPath);
    if(path == newPath)
        return;

    if(testFile.exists())
    {
        newPath.resize(newPath.size() - extension.size());
        newPath += ("(1)" + extension);
        newPath = GetNewPath(newPath);
    }
    if(!QFile::rename ( path, newPath ))
        QMessageBox::critical(this, "Ошибка", "Не удалось переименовать файл");
}

void MainWindow::on_action_10_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QString path = model->fileInfo(index).absoluteFilePath();
    if (!QFile(path).remove())
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить файл");
}

void MainWindow::on_action_2_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QDir dir = model->fileInfo(index).dir();
    QString fileName = model->fileInfo(index).fileName();
    QString path = model->fileInfo(index).absoluteFilePath();
    QString extension = "";
    QFile test(path);

    if(test.size() > USHRT_MAX)
    {
        QMessageBox::warning(this, "warning", "Sorry, file is to large");
        return;
    }

    int ext = fileName.lastIndexOf(".");
    if (ext != -1 && model->fileInfo(index).isFile())
        extension = fileName.mid(ext, fileName.length() - ext);

    if(fileName == "." || fileName == ".." || index.row() == -1 || index.column() == -1 || !model->fileInfo(index).isFile() || extension != ".txt")
    {
        QString inputPath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "*.txt;");
        if(inputPath == "")
            return;
        path = inputPath;
    }

    QString outputPath = QFileDialog::getExistingDirectory(this, "Сохранить в");
    if(outputPath == "")
        return;

    bool cancelProc;
    QString newFileName = QInputDialog::getText( this, "Назовите файл", "имя файла", QLineEdit::Normal, "compressed", &cancelProc);
    if (!cancelProc)
        return;
    if(newFileName == "")
    {
        QMessageBox::information(this, "Внимание", "Недопустимое имя файла");
        return;
    }
    outputPath += ("/" + newFileName + ".arc");
    QFile testFile(outputPath);

    if(testFile.exists())
    {
        outputPath.resize(outputPath.size() - 4);
        outputPath += "(1).arc";
        outputPath = GetNewPath(outputPath);
    }
    arc = new Archiving();
    arc->ArchiveFile(path, outputPath);
}

void MainWindow::on_action_4_triggered()
{
    QModelIndex index = ui->tableView->currentIndex();
    QDir dir = model->fileInfo(index).dir();
    QString fileName = model->fileInfo(index).fileName();
    QString path = model->fileInfo(index).absoluteFilePath();
    QString extension = "";
    int ext = fileName.lastIndexOf(".");
    if (ext != -1 && model->fileInfo(index).isFile())
        extension = fileName.mid(ext, fileName.length() - ext);

    if(fileName == "." || fileName == ".." || index.row() == -1 || index.column() == -1 || !model->fileInfo(index).isFile() || extension != ".arc")
    {
        QString inputPath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "*.txt;");
        if(inputPath == "")
            return;
        path = inputPath;
    }

    QString outputPath = QFileDialog::getExistingDirectory(this, "Извлечь в");
    if(outputPath == "")
        return;

    bool cancelProc;
    QString newFileName = QInputDialog::getText( this, "Назовите файл", "имя файла", QLineEdit::Normal, "decompressed", &cancelProc);
    if (!cancelProc)
        return;
    if(newFileName == "")
    {
        QMessageBox::information(this, "Внимание", "Недопустимое имя файла");
        return;
    }

    outputPath += ("/" + newFileName + ".txt");
    QFile testFile(outputPath);

    if(testFile.exists())
    {
        outputPath.resize(outputPath.size() - 4);
        outputPath += "(1).txt";
        outputPath = GetNewPath(outputPath);
    }
    arc = new Archiving();
    arc->DecompressFile(path, outputPath);
}
