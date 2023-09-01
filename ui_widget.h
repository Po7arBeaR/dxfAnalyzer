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
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *lab_x1;
    QLabel *lab_x2;
    QLabel *lab_y1;
    QLabel *lab_y2;

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
        label = new QLabel(Widget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 560, 21, 16));
        label_2 = new QLabel(Widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(120, 560, 21, 16));
        label_3 = new QLabel(Widget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 590, 21, 16));
        label_4 = new QLabel(Widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(120, 590, 21, 16));
        lab_x1 = new QLabel(Widget);
        lab_x1->setObjectName("lab_x1");
        lab_x1->setGeometry(QRect(30, 560, 71, 16));
        lab_x2 = new QLabel(Widget);
        lab_x2->setObjectName("lab_x2");
        lab_x2->setGeometry(QRect(30, 590, 81, 16));
        lab_y1 = new QLabel(Widget);
        lab_y1->setObjectName("lab_y1");
        lab_y1->setGeometry(QRect(150, 560, 81, 16));
        lab_y2 = new QLabel(Widget);
        lab_y2->setObjectName("lab_y2");
        lab_y2->setGeometry(QRect(150, 590, 81, 16));

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
        lab_x1->setText(QString());
        lab_x2->setText(QString());
        lab_y1->setText(QString());
        lab_y2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
