#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include <QMainWindow>
#include <QString>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include "map.h"
#include "printer.h"
#include "sensor.h"

namespace Ui {
class Wizard;
}

class Wizard : public QWizard
{
    Q_OBJECT

public:
    Wizard(map *mPtr, QWidget *parent = 0);
    explicit Wizard(QWidget *parent = 0);
    ~Wizard();

    void getModel(QStandardItemModel *model);

private slots:
    void on_missionName_lineEdit_editingFinished();
    void on_decom_lineEdit_editingFinished();
    void on_encoder_lineEdit_editingFinished();
    void on_flightLength_lineEdit_editingFinished();
    //void on_bitsPerWord_lineEdit_editingFinished();
    //void on_bitsPerWord_lineEdit_textChanged(const QString &arg1);
    void on_browse_Button_clicked();
    void on_filePath_lineEdit_textChanged();
    void clicked1(const QString &text);
    void wizardIsFinished(int result);
    void on_bitsPerWord_comboBox_currentIndexChanged(const QString &arg1);

signals:
    void clicked(const QString &text);

private:
    Ui::Wizard *ui;
    QString missionName;
    QString decom;
    QString encoder;
    QString flightLength;
    QString bitsPerWord;
    QString fileName;

    QVector<QPushButton*> v;
    QVector<QLabel*> textLine;
    QVector<sensor> tempS;
    QVector<bool> asymetricBool;
    QStandardItemModel *mainModel;
    QStandardItemModel *wizardModel;
    map *m;

    bool openFile;
};

#endif // WIZARD_H
