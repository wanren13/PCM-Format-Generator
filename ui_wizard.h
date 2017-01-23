/********************************************************************************
** Form generated from reading UI file 'wizard.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZARD_H
#define UI_WIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_Wizard
{
public:
    QWizardPage *wizardPage1;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QLineEdit *missionName_lineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *decom_lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_5;
    QLineEdit *encoder_lineEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QLineEdit *flightLength_lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_13;
    QComboBox *bitsPerWord_comboBox;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_8;
    QLabel *filePath_Label;
    QLineEdit *filePath_lineEdit;
    QPushButton *browse_Button;
    QLabel *fileNameWarning;
    QWizardPage *wizardPage;
    QTableView *tableView;
    QTextEdit *textEdit;
    QCheckBox *checkBox;

    void setupUi(QWizard *Wizard)
    {
        if (Wizard->objectName().isEmpty())
            Wizard->setObjectName(QStringLiteral("Wizard"));
        Wizard->resize(578, 424);
        Wizard->setWizardStyle(QWizard::ModernStyle);
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QStringLiteral("wizardPage1"));
        layoutWidget = new QWidget(wizardPage1);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 11, 281, 211));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_7->addWidget(label);

        missionName_lineEdit = new QLineEdit(layoutWidget);
        missionName_lineEdit->setObjectName(QStringLiteral("missionName_lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(missionName_lineEdit->sizePolicy().hasHeightForWidth());
        missionName_lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(missionName_lineEdit);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        decom_lineEdit = new QLineEdit(layoutWidget);
        decom_lineEdit->setObjectName(QStringLiteral("decom_lineEdit"));
        sizePolicy.setHeightForWidth(decom_lineEdit->sizePolicy().hasHeightForWidth());
        decom_lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(decom_lineEdit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_3->addWidget(label_5);

        encoder_lineEdit = new QLineEdit(layoutWidget);
        encoder_lineEdit->setObjectName(QStringLiteral("encoder_lineEdit"));
        sizePolicy.setHeightForWidth(encoder_lineEdit->sizePolicy().hasHeightForWidth());
        encoder_lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(encoder_lineEdit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout->addWidget(label_6);

        flightLength_lineEdit = new QLineEdit(layoutWidget);
        flightLength_lineEdit->setObjectName(QStringLiteral("flightLength_lineEdit"));
        sizePolicy.setHeightForWidth(flightLength_lineEdit->sizePolicy().hasHeightForWidth());
        flightLength_lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(flightLength_lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_2->addWidget(label_13);

        bitsPerWord_comboBox = new QComboBox(layoutWidget);
        bitsPerWord_comboBox->setObjectName(QStringLiteral("bitsPerWord_comboBox"));

        horizontalLayout_2->addWidget(bitsPerWord_comboBox);


        verticalLayout->addLayout(horizontalLayout_2);

        layoutWidget1 = new QWidget(wizardPage1);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 240, 531, 30));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        filePath_Label = new QLabel(layoutWidget1);
        filePath_Label->setObjectName(QStringLiteral("filePath_Label"));

        horizontalLayout_8->addWidget(filePath_Label);

        filePath_lineEdit = new QLineEdit(layoutWidget1);
        filePath_lineEdit->setObjectName(QStringLiteral("filePath_lineEdit"));
        filePath_lineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        horizontalLayout_8->addWidget(filePath_lineEdit);

        browse_Button = new QPushButton(layoutWidget1);
        browse_Button->setObjectName(QStringLiteral("browse_Button"));

        horizontalLayout_8->addWidget(browse_Button);

        fileNameWarning = new QLabel(wizardPage1);
        fileNameWarning->setObjectName(QStringLiteral("fileNameWarning"));
        fileNameWarning->setGeometry(QRect(110, 270, 361, 20));
        Wizard->addPage(wizardPage1);
        wizardPage = new QWizardPage();
        wizardPage->setObjectName(QStringLiteral("wizardPage"));
        tableView = new QTableView(wizardPage);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(10, 0, 540, 211));
        textEdit = new QTextEdit(wizardPage);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(0, 230, 661, 41));
        textEdit->setFrameShape(QFrame::NoFrame);
        checkBox = new QCheckBox(wizardPage);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(300, 260, 241, 21));
        Wizard->addPage(wizardPage);

        retranslateUi(Wizard);

        QMetaObject::connectSlotsByName(Wizard);
    } // setupUi

    void retranslateUi(QWizard *Wizard)
    {
        Wizard->setWindowTitle(QApplication::translate("Wizard", "Wizard", 0));
        label->setText(QApplication::translate("Wizard", "Mission Name", 0));
        label_4->setText(QApplication::translate("Wizard", "Decom Type", 0));
        label_5->setText(QApplication::translate("Wizard", "Encoder Type", 0));
        label_6->setText(QApplication::translate("Wizard", "Expected Flight Length", 0));
        label_13->setText(QApplication::translate("Wizard", "Bits per Word", 0));
        bitsPerWord_comboBox->clear();
        bitsPerWord_comboBox->insertItems(0, QStringList()
         << QApplication::translate("Wizard", "8", 0)
         << QApplication::translate("Wizard", "10", 0)
         << QApplication::translate("Wizard", "12", 0)
         << QApplication::translate("Wizard", "16", 0)
        );
        filePath_Label->setText(QApplication::translate("Wizard", "File Path & Name", 0));
        filePath_lineEdit->setText(QString());
        browse_Button->setText(QApplication::translate("Wizard", "Browse...", 0));
        fileNameWarning->setText(QString());
        textEdit->setHtml(QApplication::translate("Wizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">To make a row Asymetric click the button corresponding to the row. Each click goes back and forth from Y/N.</span></p></body></html>", 0));
        checkBox->setText(QApplication::translate("Wizard", "Open the Excel file if it is successfully created", 0));
    } // retranslateUi

};

namespace Ui {
    class Wizard: public Ui_Wizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARD_H
