#ifndef TABLEITEM_H
#define TABLEITEM_H
#include <QBitArray>

class TableItem
{
public:
    TableItem();
    TableItem(QBitArray, char);
    QBitArray Code();
    char Symbol();
private:
    QBitArray code;
    char symbol;
};

#endif // TABLEITEM_H
