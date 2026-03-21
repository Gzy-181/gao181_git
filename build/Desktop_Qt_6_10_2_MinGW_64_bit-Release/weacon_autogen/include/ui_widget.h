/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *webcon_tytle;
    QHBoxLayout *serial_layout;
    QLabel *label_2;
    QComboBox *serial_comboBox;
    QHBoxLayout *baudrate_layout;
    QLabel *baudrate_label;
    QComboBox *baundrate_comboBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *serial_open_button;
    QPushButton *serial_close_button;
    QGroupBox *groupBox;
    QWidget *layoutWidget_2;
    QHBoxLayout *temp_layout_2;
    QLabel *label_3;
    QLineEdit *lineEdit_light;
    QWidget *layoutWidget_3;
    QHBoxLayout *temp_layout_3;
    QLabel *label_4;
    QLineEdit *lineEdit_H;
    QWidget *layoutWidget_4;
    QHBoxLayout *temp_layout_4;
    QLabel *label_5;
    QLineEdit *lineEdit_hpa;
    QWidget *layoutWidget_5;
    QHBoxLayout *temp_layout_5;
    QLabel *label_6;
    QLineEdit *lineEdit_rainlevel;
    QWidget *widget1;
    QHBoxLayout *temp_layout;
    QLabel *label;
    QLineEdit *lineEdit_T;
    QHBoxLayout *horizontalLayout;
    QLabel *label_date;
    QLabel *label_time;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(669, 571);
        widget = new QWidget(Widget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(30, 20, 631, 521));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        webcon_tytle = new QLabel(widget);
        webcon_tytle->setObjectName("webcon_tytle");
        QFont font;
        font.setPointSize(27);
        webcon_tytle->setFont(font);

        gridLayout->addWidget(webcon_tytle, 0, 1, 1, 1);

        serial_layout = new QHBoxLayout();
        serial_layout->setObjectName("serial_layout");
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        QFont font1;
        font1.setPointSize(15);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        serial_layout->addWidget(label_2);

        serial_comboBox = new QComboBox(widget);
        serial_comboBox->setObjectName("serial_comboBox");

        serial_layout->addWidget(serial_comboBox);


        gridLayout->addLayout(serial_layout, 1, 0, 1, 1);

        baudrate_layout = new QHBoxLayout();
        baudrate_layout->setObjectName("baudrate_layout");
        baudrate_label = new QLabel(widget);
        baudrate_label->setObjectName("baudrate_label");
        baudrate_label->setFont(font1);
        baudrate_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        baudrate_layout->addWidget(baudrate_label);

        baundrate_comboBox = new QComboBox(widget);
        baundrate_comboBox->addItem(QString());
        baundrate_comboBox->addItem(QString());
        baundrate_comboBox->addItem(QString());
        baundrate_comboBox->setObjectName("baundrate_comboBox");

        baudrate_layout->addWidget(baundrate_comboBox);


        gridLayout->addLayout(baudrate_layout, 1, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        serial_open_button = new QPushButton(widget);
        serial_open_button->setObjectName("serial_open_button");
        serial_open_button->setMinimumSize(QSize(0, 30));
        serial_open_button->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(serial_open_button);

        serial_close_button = new QPushButton(widget);
        serial_close_button->setObjectName("serial_close_button");
        serial_close_button->setMinimumSize(QSize(0, 30));
        serial_close_button->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));

        horizontalLayout_2->addWidget(serial_close_button);


        gridLayout->addLayout(horizontalLayout_2, 1, 2, 1, 1);

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName("groupBox");
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        layoutWidget_2 = new QWidget(groupBox);
        layoutWidget_2->setObjectName("layoutWidget_2");
        layoutWidget_2->setGeometry(QRect(380, 40, 181, 52));
        temp_layout_2 = new QHBoxLayout(layoutWidget_2);
        temp_layout_2->setObjectName("temp_layout_2");
        temp_layout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget_2);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(70, 0));
        QFont font2;
        font2.setPointSize(19);
        label_3->setFont(font2);

        temp_layout_2->addWidget(label_3);

        lineEdit_light = new QLineEdit(layoutWidget_2);
        lineEdit_light->setObjectName("lineEdit_light");
        lineEdit_light->setMinimumSize(QSize(0, 50));
        lineEdit_light->setFont(font1);
        lineEdit_light->setAlignment(Qt::AlignmentFlag::AlignCenter);

        temp_layout_2->addWidget(lineEdit_light);

        layoutWidget_3 = new QWidget(groupBox);
        layoutWidget_3->setObjectName("layoutWidget_3");
        layoutWidget_3->setGeometry(QRect(90, 130, 181, 52));
        temp_layout_3 = new QHBoxLayout(layoutWidget_3);
        temp_layout_3->setObjectName("temp_layout_3");
        temp_layout_3->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget_3);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(70, 0));
        label_4->setFont(font2);

        temp_layout_3->addWidget(label_4);

        lineEdit_H = new QLineEdit(layoutWidget_3);
        lineEdit_H->setObjectName("lineEdit_H");
        lineEdit_H->setMinimumSize(QSize(0, 50));
        lineEdit_H->setFont(font1);
        lineEdit_H->setAlignment(Qt::AlignmentFlag::AlignCenter);

        temp_layout_3->addWidget(lineEdit_H);

        layoutWidget_4 = new QWidget(groupBox);
        layoutWidget_4->setObjectName("layoutWidget_4");
        layoutWidget_4->setGeometry(QRect(380, 130, 181, 52));
        temp_layout_4 = new QHBoxLayout(layoutWidget_4);
        temp_layout_4->setObjectName("temp_layout_4");
        temp_layout_4->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget_4);
        label_5->setObjectName("label_5");
        label_5->setMinimumSize(QSize(70, 0));
        label_5->setFont(font2);

        temp_layout_4->addWidget(label_5);

        lineEdit_hpa = new QLineEdit(layoutWidget_4);
        lineEdit_hpa->setObjectName("lineEdit_hpa");
        lineEdit_hpa->setMinimumSize(QSize(0, 50));
        QFont font3;
        font3.setPointSize(12);
        lineEdit_hpa->setFont(font3);
        lineEdit_hpa->setAlignment(Qt::AlignmentFlag::AlignCenter);

        temp_layout_4->addWidget(lineEdit_hpa);

        layoutWidget_5 = new QWidget(groupBox);
        layoutWidget_5->setObjectName("layoutWidget_5");
        layoutWidget_5->setGeometry(QRect(90, 220, 181, 52));
        temp_layout_5 = new QHBoxLayout(layoutWidget_5);
        temp_layout_5->setObjectName("temp_layout_5");
        temp_layout_5->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(layoutWidget_5);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(70, 0));
        label_6->setFont(font2);

        temp_layout_5->addWidget(label_6);

        lineEdit_rainlevel = new QLineEdit(layoutWidget_5);
        lineEdit_rainlevel->setObjectName("lineEdit_rainlevel");
        lineEdit_rainlevel->setMinimumSize(QSize(0, 50));
        lineEdit_rainlevel->setFont(font3);
        lineEdit_rainlevel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        temp_layout_5->addWidget(lineEdit_rainlevel);

        widget1 = new QWidget(groupBox);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(90, 40, 181, 52));
        temp_layout = new QHBoxLayout(widget1);
        temp_layout->setObjectName("temp_layout");
        temp_layout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget1);
        label->setObjectName("label");
        label->setMinimumSize(QSize(70, 0));
        label->setFont(font2);

        temp_layout->addWidget(label);

        lineEdit_T = new QLineEdit(widget1);
        lineEdit_T->setObjectName("lineEdit_T");
        lineEdit_T->setMinimumSize(QSize(0, 50));
        lineEdit_T->setFont(font1);
        lineEdit_T->setAlignment(Qt::AlignmentFlag::AlignCenter);

        temp_layout->addWidget(lineEdit_T);


        gridLayout->addWidget(groupBox, 2, 0, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_date = new QLabel(widget);
        label_date->setObjectName("label_date");
        label_date->setFont(font3);
        label_date->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label_date);

        label_time = new QLabel(widget);
        label_time->setObjectName("label_time");
        label_time->setFont(font3);
        label_time->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label_time);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 3);

        gridLayout->setRowStretch(0, 1);
        gridLayout->setRowStretch(1, 1);
        gridLayout->setRowStretch(2, 4);
        gridLayout->setRowStretch(3, 1);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        webcon_tytle->setText(QCoreApplication::translate("Widget", "\345\276\256\345\236\213\346\260\224\350\261\241\347\253\231", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\344\270\262\345\217\243\357\274\232", nullptr));
        baudrate_label->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        baundrate_comboBox->setItemText(0, QCoreApplication::translate("Widget", "4800", nullptr));
        baundrate_comboBox->setItemText(1, QCoreApplication::translate("Widget", "9600", nullptr));
        baundrate_comboBox->setItemText(2, QCoreApplication::translate("Widget", "115200", nullptr));

        serial_open_button->setText(QCoreApplication::translate("Widget", "\350\277\236\346\216\245", nullptr));
        serial_close_button->setText(QCoreApplication::translate("Widget", "\346\226\255\345\274\200", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Widget", "\346\260\224\350\261\241\346\225\260\346\215\256\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\345\205\211\345\274\272\357\274\232", nullptr));
        lineEdit_light->setText(QCoreApplication::translate("Widget", "  xx lx", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", " \346\271\277\345\272\246\357\274\232", nullptr));
        lineEdit_H->setText(QCoreApplication::translate("Widget", "  xx %", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", " \346\260\224\345\216\213 \357\274\232", nullptr));
        lineEdit_hpa->setText(QCoreApplication::translate("Widget", "  xx hpa", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", " \351\231\215\346\260\264\357\274\232", nullptr));
        lineEdit_rainlevel->setText(QCoreApplication::translate("Widget", "  xxxx", nullptr));
        label->setText(QCoreApplication::translate("Widget", " \346\270\251\345\272\246\357\274\232", nullptr));
        lineEdit_T->setText(QCoreApplication::translate("Widget", "  xx\342\204\203", nullptr));
        label_date->setText(QCoreApplication::translate("Widget", "2026-3-9", nullptr));
        label_time->setText(QCoreApplication::translate("Widget", "17:17:42", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
