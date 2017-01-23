#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLayout>
#include <QMessageBox>
#include "parser.h"
#include "printer.h"
#include "wizard.h"
#include "helpdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tableView);
    success = false;
    // Most applications start with a new page
    on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//the user wants to create a new map
void MainWindow::on_actionNew_triggered()
{
    success = false;
    newTable = true;
    fileName = "";
    this->setWindowTitle(fileName);
    p.clearData();
    populateTable();
}

//the user wants to open an existing map
void MainWindow::on_actionOpen_triggered()
{
    newTable = false;   // Only true when New button clicked
    success = false;
    p.clearData();
    m.clearData();

    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open a Measurement List"),
                                            ".",
                                            tr("Excel Files (*.xlsx);;Text Files (*.txt);;Any Files (*)"));

    // Cannot open an empty fileName
    // Only a empty fileName when Cancel is clicked
    if(!fileName.isEmpty())
    {
        success = p.LoadParser(fileName);

        if(!success)
            QMessageBox::warning(this, "Error", "Wrong format");
        else
        {
            populateTable();
            this->setWindowTitle(fileName);
        }
    }
    else
        on_actionNew_triggered();   // resets newTable bool
}

void MainWindow::print()
{
    printTable();

    xlsx.saveAs(fileName);
}

void MainWindow::printTable()
{
    xlsx.addSheet("Table");

    QXlsx::Format format;
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    xlsx.write(1, 1, "Channel", format);
    xlsx.write(1, 2, "Description", format);
    xlsx.write(1, 3, "Mnemonic", format);
    xlsx.write(1, 4, "Minimum Samples/second", format);

    for(int row=0; row<model->rowCount(); row++){

        QString channel1 = model->item(row,0)->text();
        QString description1 = model->item(row,1)->text();
        QString mnemonic1 = model->item(row,2)->text();
        QString rss1 = model->item(row,3)->text();

        int r = row+2;

        xlsx.write(r, 1, channel1, format);
        xlsx.write(r, 2, description1, format);
        xlsx.write(r, 3, mnemonic1, format);
        xlsx.write(r, 4, rss1, format);
    }
}

void MainWindow::clearExcelTable()
{
    xlsx.addSheet("Table");


    xlsx.write(1, 1, "Channel");
    xlsx.write(1, 2, "Description");
    xlsx.write(1, 3, "Mnemonic");
    xlsx.write(1, 4, "Minimum Samples/second");

    for(int row=0; row<rowCountBeforeRowDelete; row++){

        int r = row+2;

        xlsx.write(r, 1, "");
        xlsx.write(r, 2, "");
        xlsx.write(r, 3, "");
        xlsx.write(r, 4, "");
    }

}

void MainWindow::on_actionSave_triggered()
{
    if(fileName != "")
    {
        if( success | newTable )   // print needs a model
        {
            clearExcelTable();
            xlsx.saveAs(fileName);
            print();
        }
    }
    else
        on_actionSave_As_triggered();   // if saved clicked first

}

void MainWindow::on_actionSave_As_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Open a file");

    if(!file.isEmpty())
    {
        if(!file.endsWith(".xlsx"))
            fileName = file + ".xlsx";
        else
            fileName = file;

        this->setWindowTitle(fileName); // up-date title

        on_actionSave_triggered();
    }
}

void MainWindow::on_actionAbout_MapGenerator_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{
    /*
    HelpDialog hDialog;
    hDialog.setModal(true);
    hDialog.exec();
    */
    hDialog = new HelpDialog(this);
    hDialog->show();
}

void MainWindow::populateTable()
{
    //if a file was loaded
    if( success )
    {
        int row = p.getNumOfRows();
        int col = p.getNumOfCols();
        //QStandardItemModel( rows, columns, parent )
        model = new QStandardItemModel(row, col, this);

        for(int i=0;i<col;i++)
        {
            model->setHorizontalHeaderItem(i, new QStandardItem(QString(p.getTitleAt(i))));
        }
        //for each row of data
        for( int r=0; r<row; r++ )
        {
            //for each column
            for( int c=0; c<col; c++ )
            {
                QModelIndex index = model->index(r, c, QModelIndex());

                QString string = p.getEntry(r,c);
                if(c == 1 || c == 2)
                    model->setData(index, Qt::AlignLeft + Qt::AlignVCenter, Qt::TextAlignmentRole);
                else
                    model->setData(index, Qt::AlignCenter, Qt::TextAlignmentRole);

                if ( c == col - 1 )
                {
                   int data = string.toInt();
                   model->setData(index, data);

                }
                else
                    model->setData(index, string);
            }
        }
    }
    //create an empty matrix
    else
    {
        model = new QStandardItemModel(1, 4, this);
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Description")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Format Label")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Type")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Minimum Samples/second")));
    }

    //set up our table view based off the above matrix
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 700);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 100);


    //link rightclick menu for our table
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    //link rightclick menu for our header
    QHeaderView *verticalHeader;
    verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(verticalHeader, SIGNAL(customContextMenuRequested(const QPoint&)),
            this,SLOT(ShowContextMenu(const QPoint&)));

    // For deleting and saving purposes
    rowCountBeforeRowDelete = model->rowCount();
}

// Open Generate Map Wizard (Create > Generate Map)
void MainWindow::on_actionGenerate_Map_2_triggered()
{
    // probably a better way to do this
    // but theres two ways to click generate map now
    on_actionGenerate_Map_triggered();
}

// Open Generate Map Wizard (Generate Map)
void MainWindow::on_actionGenerate_Map_triggered()
{
    bool complete = true;
    for (int r = 0; r<model->rowCount(); r++){
        if(model->item(r,0) != NULL){
            if(model->item(r,0)->text().isEmpty()){
                complete = false;
                break;
            }
        }
        else{
            complete = false;
            break;
        }

        if(model->item(r,3) != NULL){
            if(model->item(r,3)->text().isEmpty()){
                complete = false;
                break;
            }
        }
        else{
            complete = false;
            break;
        }
    }


    if(!complete)
        QMessageBox::warning(this, "Error", "Table is incomplete!");
    else{
        if( success | newTable)
        {
            m.clearBitRateAdder();
            m.clearData();
            Wizard mDialog(&m);
            mDialog.setModal(true);
            mDialog.getModel(model);
            mDialog.exec();
        }
    }
}

void MainWindow::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);

    QMenu rightClickMenu;
    rightClickMenu.addAction("Add Row");
    rightClickMenu.addAction("Delete Row");

    QAction* selectedItem = rightClickMenu.exec(globalPos);
    if (selectedItem)
    {
        // something was chosen, do stuff
        QString temp = selectedItem->text();
        if(temp == "Add Row")
        {
            ui->tableView->model()->insertRow(model->rowCount());

            QModelIndex index = model->index(model->rowCount()-1, 0, QModelIndex());
            model->setData(index, "");

            index = model->index(model->rowCount()-1, 1, QModelIndex());
            model->setData(index, "");

            index = model->index(model->rowCount()-1, 2, QModelIndex());
            model->setData(index, "");

            index = model->index(model->rowCount()-1, 3, QModelIndex());
            model->setData(index, "");
        }
        else if(temp == "Delete Row")
        {
            deleteRows();
        }
    }
    else
    {
        // nothing was chosen
    }
}

void MainWindow::deleteRows()
{
    rowCountBeforeRowDelete = model->rowCount();

    //grab selection
    QItemSelection selection = ui->tableView->selectionModel()->selection();
    //find out the selected rows
    QList<int> removeRows;
    foreach(QModelIndex index, selection.indexes())
    {
        if(!removeRows.contains(index.row()))
        {
            removeRows.append(index.row());
        }
    }

    //loop through selected rows and remove them
    for(int i=0;i<removeRows.count();++i)
    {
        //decrement all rows after the current
        //as the row number will change if we remove
        for(int j=1;j<removeRows.count();++j)
        {
            if(removeRows.at(j) > removeRows.at(i))
            {
                removeRows[j]--;
            }
        }
        ui->tableView->model()->removeRows(removeRows.at(i),1);
        //ui->tableView->model()->endRemoveRows();
    }

}

// Close Application - I guess the big x to the right
// wasn't sufficient enough for some folks :/
void MainWindow::on_actionClose_triggered()
{
    QCoreApplication::exit();
}

