#include "wizard.h"
#include "ui_wizard.h"

#include <QMainWindow>
#include <QString>
#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QSignalMapper>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QColor>
#include <QFont>

Wizard::Wizard(map *mPtr, QWidget *parent):
    QWizard(parent),
    ui(new Ui::Wizard),
    m(mPtr)
{

    QObject::connect(this, SIGNAL(finished (int)), this, SLOT(wizardIsFinished(int)));

    ui->setupUi(this);
    fileName        = QDir::currentPath();
    ui->filePath_lineEdit->setText(fileName.append("/"));
    //ui->bitsPerWord_lineEdit->setStyleSheet("background-color: yellow");
    bitsPerWord = "8";
    missionName = "mission";
}

Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    missionName = "mission";
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::wizardIsFinished(int result){
   if(result == QDialog::Accepted){

       m->setMissionName(missionName);
       m->setDecom(decom);
       m->setEncoder(encoder);
       m->setDecom(decom);
       m->setBitsPerWord(bitsPerWord.toInt());
       m->setOutFile(fileName);

       m->addSensors(tempS);
       m->setAsymmetricSensor(asymetricBool);

       if(m->createMap()){
           printer *mPrinter = new printer(mainModel, m);
           mPrinter->print();
           delete mPrinter;

           if(ui->checkBox->isChecked())
               system(qPrintable(m->getOutFile())); //  Sorry, I didn't find a way to suppress the console window
       }
   }

}

// Mission Name
void Wizard::on_missionName_lineEdit_editingFinished()
{
    missionName = ui->missionName_lineEdit->text();
}

// Decom
void Wizard::on_decom_lineEdit_editingFinished()
{
    decom = ui->decom_lineEdit->text();
}

// Encoder
void Wizard::on_encoder_lineEdit_editingFinished()
{
    encoder = ui->encoder_lineEdit->text();
}

// Expected Length Of Flight
void Wizard::on_flightLength_lineEdit_editingFinished()
{
    flightLength = ui->flightLength_lineEdit->text();
}


void Wizard::on_bitsPerWord_comboBox_currentIndexChanged(const QString &arg1)
{
    bitsPerWord = ui->bitsPerWord_comboBox->currentText();
}

/*
 * Gone but not forgotten :(
 *
// bit per word
void Wizard::on_bitsPerWord_lineEdit_editingFinished()
{
    bitsPerWord = ui->bitsPerWord_lineEdit->text();
}


void Wizard::on_bitsPerWord_lineEdit_textChanged(const QString &arg1)
{
    bitsPerWord = ui->bitsPerWord_lineEdit->text();

    if( bitsPerWord == "8"  |
        bitsPerWord == "10" |
        bitsPerWord == "12" |
        bitsPerWord == "16" )
    {
        ui->bitsPerWord_lineEdit->setStyleSheet("background-color: cyan");
        ui->validBitLabel->setText("");
        ui->validBitLabel->setStyleSheet("background-color: white");
    }
    else
    {
        ui->bitsPerWord_lineEdit->setStyleSheet("background-color: red");
        ui->validBitLabel->setText("Invalid Bit!  Try: 8, 10, 12, 16");
        ui->validBitLabel->setStyleSheet("background-color: yellow");
    }
}
*/

void Wizard::on_browse_Button_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Save",
                                                QDir::currentPath(),
                                                "*.xlsx");

    if (!file.isEmpty())
    {
        if(!file.endsWith(".xlsx"))
            fileName = file + ".xlsx";
        else
            fileName = file;

        ui->filePath_lineEdit->setText(fileName);
    }
}

void Wizard::on_filePath_lineEdit_textChanged()
{

    fileName = ui->filePath_lineEdit->text();

    if( fileName.endsWith("/") )
    {
        fileName += missionName + "_OutPut.xlsx";
        ui->fileNameWarning->setStyleSheet("background-color: yellow");
        ui->fileNameWarning->setText("                                       Enter a unique file name");
    }
    else
    {
        ui->fileNameWarning->setStyleSheet("background-color: white");
        ui->fileNameWarning->setText(" ");
    }
}

// Get Model Data
void Wizard::getModel(QStandardItemModel *model)
{

    mainModel = model;

    int rows = model->rowCount();
    int cols = model->columnCount();

    wizardModel = new QStandardItemModel(rows, 5, this);

    for(int r=0; r<rows; r++)
    {
        for(int c=0; c<cols; c++)
        {
            QModelIndex index = wizardModel->index(r, c);
            QModelIndex index1 = model->index(r, c);
            QVariant data = model->data(index1);
            QString text = data.toString();
            switch(c)
            {
            case 0:
                wizardModel->setData(index, text);
                wizardModel->setData(index, Qt::AlignCenter, Qt::TextAlignmentRole);
                break;
            case 1:
                wizardModel->setData(index, text);
                wizardModel->setData(index, Qt::AlignLeft + Qt::AlignVCenter, Qt::TextAlignmentRole);
                break;
            case 2:
                wizardModel->setData(index, text);
                break;
            case 3:
                wizardModel->setData(index, text.toInt());
                break;
            default:
                break;
            }
        }
    }

    wizardModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Channel")));
    wizardModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Description")));
    wizardModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Mnemonic")));
    wizardModel->setHorizontalHeaderItem(3, new QStandardItem(QString("SPS")));
    wizardModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Asymetrical?")));
    wizardModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Y/N")));

    wizardModel->sort(3, Qt::DescendingOrder);

    sensor tempSensor;
    for(int r=0; r<wizardModel->rowCount(); r++)
    {
        for(int c=0; c<wizardModel->columnCount(); c++)
        {
            switch(c)
            {
            case 0:
                tempSensor.setTag(wizardModel->item(r,c)->text());
                break;
            case 1:
                tempSensor.setDescription(wizardModel->item(r,c)->text());
                break;
            case 2:
                tempSensor.setMnemonic(wizardModel->item(r,c)->text());
                break;
            case 3:
                tempSensor.setMinSPS(wizardModel->item(r,c)->text().toInt());
                break;
            default:
                break;
            }
        }
        tempS.append(tempSensor);
    }

    ui->tableView->setModel(wizardModel);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);


    QSignalMapper *signalMapper = new QSignalMapper(this);
    //connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(digitClicked(int)));

    for(int r=0; r<rows; r++)
    {
        // Vector of buttons
        v.push_back(new QPushButton(QString::number(r+1), this));

        signalMapper->setMapping(v.at(r), QString::number(r+1));
        connect(v.at(r), SIGNAL(clicked()), signalMapper, SLOT(map()));

        // Set the button
        ui->tableView->setIndexWidget(wizardModel->index(r, 4), v.at(r));

        // Set the label
        textLine.push_back(new QLabel("No", this));
        ui->tableView->setIndexWidget(wizardModel->index(r, 5), textLine.at(r));

        // Rows are automatically non-Asymetric
        asymetricBool.push_back(false);
    }

    connect(signalMapper, SIGNAL(mapped(QString)),
            this, SLOT(clicked1(QString)));

    // Check all rows for Video data
    for(int r=0; r<wizardModel->rowCount(); r++)
    {
        if(wizardModel->item(r,1)->text().toLower().contains("video")){
            asymetricBool.replace(0, true);
            textLine.at(0)->setText("Yes");

            QColor rowColor = Qt::yellow;
            wizardModel->item(r,0)->setData(rowColor, Qt::BackgroundRole);
            wizardModel->item(r,1)->setData(rowColor, Qt::BackgroundRole);
            wizardModel->item(r,3)->setData(rowColor, Qt::BackgroundRole);
        }
    }

    ui->tableView->setColumnWidth(0, 60);
    ui->tableView->setColumnWidth(1, 300);
    ui->tableView->setColumnWidth(4, 80);
    ui->tableView->setColumnWidth(5, 50);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void Wizard::clicked1(const QString &text)
{
    // Essentially a row index
    int tempIndex = text.toInt()-1;

    if(asymetricBool.at(tempIndex))
    {
        asymetricBool.replace(tempIndex, false);
        textLine.at(tempIndex)->setText("No");

        QColor rowColor = Qt::white;
        wizardModel->item(tempIndex,0)->setData(rowColor, Qt::BackgroundRole);
        wizardModel->item(tempIndex,1)->setData(rowColor, Qt::BackgroundRole);
        wizardModel->item(tempIndex,3)->setData(rowColor, Qt::BackgroundRole);
    }
    else
    {
        asymetricBool.replace(tempIndex, true);
        textLine.at(tempIndex)->setText("Yes");

        QColor rowColor = Qt::yellow;
        wizardModel->item(tempIndex,0)->setData(rowColor, Qt::BackgroundRole);
        wizardModel->item(tempIndex,1)->setData(rowColor, Qt::BackgroundRole);
        wizardModel->item(tempIndex,3)->setData(rowColor, Qt::BackgroundRole);
    }
}
