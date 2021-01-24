#include "tableitem.h"

TableItem::TableItem()
{

}

TableItem::TableItem(QBitArray code, char symbol)
{
    this->code = code;
    this->symbol = symbol;
}

QBitArray TableItem::Code()
{
    return this->code;
}
char TableItem::Symbol()
{
    return this->symbol;
}
