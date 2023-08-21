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

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(849, 600);
        PB_Load = new QPushButton(Widget);
        PB_Load->setObjectName("PB_Load");
        PB_Load->setGeometry(QRect(10, 20, 80, 24));
        tw_AnalyzedData = new QTableWidget(Widget);
        tw_AnalyzedData->setObjectName("tw_AnalyzedData");
        tw_AnalyzedData->setGeometry(QRect(10, 50, 191, 521));
        pb_output = new QPushButton(Widget);
        pb_output->setObjectName("pb_output");
        pb_output->setGeometry(QRect(110, 20, 80, 24));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        PB_Load->setText(QCoreApplication::translate("Widget", "\345\257\274\345\205\245", nullptr));
        pb_output->setText(QCoreApplication::translate("Widget", "\350\276\223\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
