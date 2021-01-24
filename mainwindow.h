#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QFileSystemModel>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
//#include <QTextStream>
#include <queue>
//#include <list>
#include "archiving.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_action_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_action_9_triggered();

    void on_action_12_triggered();

    void on_action_11_triggered();

    void on_action_10_triggered();

    void on_action_2_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    Archiving *arc;
};

#endif // MAINWINDOW_H
