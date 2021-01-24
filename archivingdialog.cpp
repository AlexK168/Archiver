#include "archivingdialog.h"
#include "ui_archivingdialog.h"

ArchivingDialog::ArchivingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchivingDialog)
{
    ui->setupUi(this);
}

ArchivingDialog::~ArchivingDialog()
{
    delete ui;
}
