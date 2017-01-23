#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtCore>
#include <QtGui>
#include "parser.h"
#include "map.h"
#include "xlsxdocument.h"
#include "helpdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void print();
    void printTable();
    void clearExcelTable();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionAbout_MapGenerator_triggered();

    void on_actionAbout_triggered();

    void on_actionGenerate_Map_triggered();

    void on_actionGenerate_Map_2_triggered();

    void on_actionClose_triggered();

    void ShowContextMenu(const QPoint& pos);

    void deleteRows();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QXlsx::Document xlsx;
    HelpDialog *hDialog;

    parser p;
    map m;

    QString fileName;

    bool success;
    bool newTable;
    int rowCountBeforeRowDelete;

    void populateTable();
};

#endif // MAINWINDOW_H
