#ifndef ARCHIVINGDIALOG_H
#define ARCHIVINGDIALOG_H

#include <QDialog>

namespace Ui {
class ArchivingDialog;
}

class ArchivingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArchivingDialog(QWidget *parent = nullptr);
    ~ArchivingDialog();

private:
    Ui::ArchivingDialog *ui;
};

#endif // ARCHIVINGDIALOG_H
