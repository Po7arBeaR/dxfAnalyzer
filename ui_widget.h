/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *PB_Load;
    QTableWidget *tw_AnalyzedData;
    QPushButton *pb_output;
    QPushButton *pb_Config;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(849, 612);
        PB_Load = new QPushButton(Widget);
        PB_Load->setObjectName("PB_Load");
        PB_Load->setGeometry(QRect(0, 0, 80, 24));
        tw_AnalyzedData = new QTableWidget(Widget);
        tw_AnalyzedData->setObjectName("tw_AnalyzedData");
        tw_AnalyzedData->setGeometry(QRect(0, 30, 231, 521));
        pb_output = new QPushButton(Widget);
        pb_output->setObjectName("pb_output");
        pb_output->setGeometry(QRect(80, 0, 80, 24));
        pb_Config = new QPushButton(Widget);
        pb_Config->setObjectName("pb_Config");
        pb_Config->setGeometry(QRect(160, 0, 71, 24));
        lineEdit = new QLineEdit(Widget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(20, 550, 91, 23));
        lineEdit_2 = new QLineEdit(Widget);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(140, 550, 91, 23));
        label = new QLabel(Widget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 550, 21, 16));
        label_2 = new QLabel(Widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(120, 550, 21, 16));
        lineEdit_3 = new QLineEdit(Widget);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(140, 580, 91, 23));
        lineEdit_4 = new QLineEdit(Widget);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(20, 582, 91, 21));
        label_3 = new QLabel(Widget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 580, 21, 16));
        label_4 = new QLabel(Widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(120, 580, 21, 16));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        PB_Load->setText(QCoreApplication::translate("Widget", "\345\257\274\345\205\245", nullptr));
        pb_output->setText(QCoreApplication::translate("Widget", "\350\276\223\345\207\272", nullptr));
        pb_Config->setText(QCoreApplication::translate("Widget", "\351\205\215\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("Widget", "x1:", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "y1:", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "x2:", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "y2:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
