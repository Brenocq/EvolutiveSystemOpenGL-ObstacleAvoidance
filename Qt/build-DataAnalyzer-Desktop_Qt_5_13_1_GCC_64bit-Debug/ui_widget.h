/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *fileManagerBox;
    QPushButton *importButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *fileNameEdit;
    QLineEdit *qtdRobotsEdit;
    QLineEdit *qtdEnvEdit;
    QLineEdit *qtdRepEdit;
    QLineEdit *qtdObstEdit;
    QLineEdit *maxPopEdit;
    QLineEdit *maxTimeEdit;
    QLineEdit *envMutEdit;
    QLineEdit *pointsColEdit;
    QLineEdit *pointsMovEdit;
    QTabWidget *tabWidget;
    QWidget *robotEvolution;
    QComboBox *envComboBox;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QComboBox *genComboBox;
    QComboBox *repComboBox;
    QLabel *label_24;
    QComboBox *selRobotComboBox;
    QLabel *label_25;
    QLabel *label_26;
    QPushButton *allVisibleButton;
    QPushButton *allInvisibleButton;
    QCheckBox *visibleCheckBox;
    QWidget *envEvolution;
    QCustomPlot *graphGross;
    QCustomPlot *graphMean;
    QCustomPlot *graphSensors;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(900, 901);
        fileManagerBox = new QGroupBox(Widget);
        fileManagerBox->setObjectName(QString::fromUtf8("fileManagerBox"));
        fileManagerBox->setGeometry(QRect(20, 10, 221, 271));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        fileManagerBox->setFont(font);
        fileManagerBox->setAlignment(Qt::AlignCenter);
        importButton = new QPushButton(fileManagerBox);
        importButton->setObjectName(QString::fromUtf8("importButton"));
        importButton->setGeometry(QRect(60, 30, 89, 25));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        importButton->setFont(font1);
        label = new QLabel(fileManagerBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 60, 131, 17));
        label->setFont(font1);
        label_2 = new QLabel(fileManagerBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 80, 131, 17));
        label_2->setFont(font1);
        label_3 = new QLabel(fileManagerBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 120, 131, 17));
        label_3->setFont(font1);
        label_4 = new QLabel(fileManagerBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 140, 131, 17));
        label_4->setFont(font1);
        label_5 = new QLabel(fileManagerBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 160, 131, 17));
        label_5->setFont(font1);
        label_6 = new QLabel(fileManagerBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 180, 131, 17));
        label_6->setFont(font1);
        label_7 = new QLabel(fileManagerBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 200, 131, 17));
        label_7->setFont(font1);
        label_8 = new QLabel(fileManagerBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 220, 131, 17));
        label_8->setFont(font1);
        label_9 = new QLabel(fileManagerBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 240, 131, 17));
        label_9->setFont(font1);
        label_10 = new QLabel(fileManagerBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 100, 131, 17));
        label_10->setFont(font1);
        fileNameEdit = new QLineEdit(fileManagerBox);
        fileNameEdit->setObjectName(QString::fromUtf8("fileNameEdit"));
        fileNameEdit->setGeometry(QRect(120, 60, 91, 21));
        fileNameEdit->setFont(font1);
        fileNameEdit->setReadOnly(true);
        qtdRobotsEdit = new QLineEdit(fileManagerBox);
        qtdRobotsEdit->setObjectName(QString::fromUtf8("qtdRobotsEdit"));
        qtdRobotsEdit->setGeometry(QRect(160, 80, 51, 21));
        qtdRobotsEdit->setFont(font1);
        qtdRobotsEdit->setReadOnly(true);
        qtdEnvEdit = new QLineEdit(fileManagerBox);
        qtdEnvEdit->setObjectName(QString::fromUtf8("qtdEnvEdit"));
        qtdEnvEdit->setGeometry(QRect(160, 100, 51, 21));
        qtdEnvEdit->setFont(font1);
        qtdEnvEdit->setReadOnly(true);
        qtdRepEdit = new QLineEdit(fileManagerBox);
        qtdRepEdit->setObjectName(QString::fromUtf8("qtdRepEdit"));
        qtdRepEdit->setGeometry(QRect(160, 120, 51, 21));
        qtdRepEdit->setFont(font1);
        qtdRepEdit->setReadOnly(true);
        qtdObstEdit = new QLineEdit(fileManagerBox);
        qtdObstEdit->setObjectName(QString::fromUtf8("qtdObstEdit"));
        qtdObstEdit->setGeometry(QRect(160, 140, 51, 21));
        qtdObstEdit->setFont(font1);
        qtdObstEdit->setReadOnly(true);
        maxPopEdit = new QLineEdit(fileManagerBox);
        maxPopEdit->setObjectName(QString::fromUtf8("maxPopEdit"));
        maxPopEdit->setGeometry(QRect(160, 200, 51, 21));
        maxPopEdit->setFont(font1);
        maxPopEdit->setReadOnly(true);
        maxTimeEdit = new QLineEdit(fileManagerBox);
        maxTimeEdit->setObjectName(QString::fromUtf8("maxTimeEdit"));
        maxTimeEdit->setGeometry(QRect(160, 220, 51, 21));
        maxTimeEdit->setFont(font1);
        maxTimeEdit->setReadOnly(true);
        envMutEdit = new QLineEdit(fileManagerBox);
        envMutEdit->setObjectName(QString::fromUtf8("envMutEdit"));
        envMutEdit->setGeometry(QRect(160, 240, 51, 21));
        envMutEdit->setFont(font1);
        envMutEdit->setReadOnly(true);
        pointsColEdit = new QLineEdit(fileManagerBox);
        pointsColEdit->setObjectName(QString::fromUtf8("pointsColEdit"));
        pointsColEdit->setGeometry(QRect(160, 160, 51, 21));
        pointsColEdit->setFont(font1);
        pointsColEdit->setReadOnly(true);
        pointsMovEdit = new QLineEdit(fileManagerBox);
        pointsMovEdit->setObjectName(QString::fromUtf8("pointsMovEdit"));
        pointsMovEdit->setGeometry(QRect(160, 180, 51, 21));
        pointsMovEdit->setFont(font1);
        pointsMovEdit->setReadOnly(true);
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(260, 10, 621, 271));
        tabWidget->setFont(font);
        robotEvolution = new QWidget();
        robotEvolution->setObjectName(QString::fromUtf8("robotEvolution"));
        envComboBox = new QComboBox(robotEvolution);
        envComboBox->setObjectName(QString::fromUtf8("envComboBox"));
        envComboBox->setGeometry(QRect(120, 30, 91, 25));
        envComboBox->setFont(font1);
        label_21 = new QLabel(robotEvolution);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(20, 30, 101, 17));
        label_21->setFont(font1);
        label_22 = new QLabel(robotEvolution);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 10, 191, 17));
        QFont font2;
        font2.setBold(false);
        font2.setItalic(false);
        font2.setUnderline(true);
        font2.setWeight(50);
        font2.setKerning(true);
        label_22->setFont(font2);
        label_23 = new QLabel(robotEvolution);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(20, 60, 101, 17));
        label_23->setFont(font1);
        genComboBox = new QComboBox(robotEvolution);
        genComboBox->setObjectName(QString::fromUtf8("genComboBox"));
        genComboBox->setEnabled(false);
        genComboBox->setGeometry(QRect(120, 60, 91, 25));
        genComboBox->setFont(font1);
        repComboBox = new QComboBox(robotEvolution);
        repComboBox->setObjectName(QString::fromUtf8("repComboBox"));
        repComboBox->setEnabled(false);
        repComboBox->setGeometry(QRect(120, 90, 91, 25));
        repComboBox->setFont(font1);
        label_24 = new QLabel(robotEvolution);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(20, 90, 101, 17));
        label_24->setFont(font1);
        selRobotComboBox = new QComboBox(robotEvolution);
        selRobotComboBox->setObjectName(QString::fromUtf8("selRobotComboBox"));
        selRobotComboBox->setEnabled(false);
        selRobotComboBox->setGeometry(QRect(370, 70, 91, 25));
        selRobotComboBox->setFont(font1);
        label_25 = new QLabel(robotEvolution);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(250, 10, 191, 17));
        label_25->setFont(font2);
        label_26 = new QLabel(robotEvolution);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(270, 70, 101, 17));
        label_26->setFont(font1);
        allVisibleButton = new QPushButton(robotEvolution);
        allVisibleButton->setObjectName(QString::fromUtf8("allVisibleButton"));
        allVisibleButton->setEnabled(false);
        allVisibleButton->setGeometry(QRect(270, 30, 89, 25));
        allVisibleButton->setFont(font1);
        allInvisibleButton = new QPushButton(robotEvolution);
        allInvisibleButton->setObjectName(QString::fromUtf8("allInvisibleButton"));
        allInvisibleButton->setEnabled(false);
        allInvisibleButton->setGeometry(QRect(370, 30, 89, 25));
        allInvisibleButton->setFont(font1);
        visibleCheckBox = new QCheckBox(robotEvolution);
        visibleCheckBox->setObjectName(QString::fromUtf8("visibleCheckBox"));
        visibleCheckBox->setEnabled(false);
        visibleCheckBox->setGeometry(QRect(290, 100, 92, 23));
        visibleCheckBox->setFont(font1);
        tabWidget->addTab(robotEvolution, QString());
        envEvolution = new QWidget();
        envEvolution->setObjectName(QString::fromUtf8("envEvolution"));
        tabWidget->addTab(envEvolution, QString());
        graphGross = new QCustomPlot(Widget);
        graphGross->setObjectName(QString::fromUtf8("graphGross"));
        graphGross->setGeometry(QRect(20, 300, 839, 191));
        graphMean = new QCustomPlot(Widget);
        graphMean->setObjectName(QString::fromUtf8("graphMean"));
        graphMean->setGeometry(QRect(20, 500, 839, 191));
        graphSensors = new QCustomPlot(Widget);
        graphSensors->setObjectName(QString::fromUtf8("graphSensors"));
        graphSensors->setGeometry(QRect(20, 700, 839, 191));

        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        fileManagerBox->setTitle(QCoreApplication::translate("Widget", "File Manager", nullptr));
        importButton->setText(QCoreApplication::translate("Widget", "Import File", nullptr));
        label->setText(QCoreApplication::translate("Widget", "File name :", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "Qtd robots:", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "Qtd repetitions:", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "Qtd obstacles:", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "Points collision:", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "Points moving:", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "Max population:", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "Max time:", nullptr));
        label_9->setText(QCoreApplication::translate("Widget", "Env mutation rate:", nullptr));
        label_10->setText(QCoreApplication::translate("Widget", "Qtd environemnts:", nullptr));
        label_21->setText(QCoreApplication::translate("Widget", "Environment:", nullptr));
        label_22->setText(QCoreApplication::translate("Widget", "Select population", nullptr));
        label_23->setText(QCoreApplication::translate("Widget", "Generation:", nullptr));
        label_24->setText(QCoreApplication::translate("Widget", "Repetition:", nullptr));
        label_25->setText(QCoreApplication::translate("Widget", "Graph visualization", nullptr));
        label_26->setText(QCoreApplication::translate("Widget", "Select robot:", nullptr));
        allVisibleButton->setText(QCoreApplication::translate("Widget", "All Visible", nullptr));
        allInvisibleButton->setText(QCoreApplication::translate("Widget", "All Invisible", nullptr));
        visibleCheckBox->setText(QCoreApplication::translate("Widget", "Visible", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(robotEvolution), QCoreApplication::translate("Widget", "Robot Evolution", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(envEvolution), QCoreApplication::translate("Widget", "Environment Evolution", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
