#ifndef ARCHIVING_H
#define ARCHIVING_H

#include <QFile>
#include <QListView>
#include <QStringListModel>
#include <QTextStream>
#include <queue>
#include <QMessageBox>
#include <limits.h>
#include "comparenode.h"
#include "tree.h"
#include "tableitem.h"
class Archiving
{
public:
    Archiving();
    void ArchiveFile(QString, QString);
    void DecompressFile(QString, QString);
};

#endif // ARCHIVING_H
