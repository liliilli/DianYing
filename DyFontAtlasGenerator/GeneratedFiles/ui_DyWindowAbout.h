/********************************************************************************
** Form generated from reading UI file 'DyWindowAbout.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DYWINDOWABOUT_H
#define UI_DYWINDOWABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DyWindowAbout
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QListView *LV_Library;
    QPushButton *PB_OK;
    QGraphicsView *GV_Logo;
    QPushButton *PB_Log;

    void setupUi(QWidget *DyWindowAbout)
    {
        if (DyWindowAbout->objectName().isEmpty())
            DyWindowAbout->setObjectName(QStringLiteral("DyWindowAbout"));
        DyWindowAbout->resize(480, 320);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DyWindowAbout->sizePolicy().hasHeightForWidth());
        DyWindowAbout->setSizePolicy(sizePolicy);
        label = new QLabel(DyWindowAbout);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 120, 201, 16));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_2 = new QLabel(DyWindowAbout);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 140, 201, 16));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_3 = new QLabel(DyWindowAbout);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 160, 281, 16));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_4 = new QLabel(DyWindowAbout);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 190, 191, 16));
        LV_Library = new QListView(DyWindowAbout);
        LV_Library->setObjectName(QStringLiteral("LV_Library"));
        LV_Library->setGeometry(QRect(10, 210, 360, 101));
        PB_OK = new QPushButton(DyWindowAbout);
        PB_OK->setObjectName(QStringLiteral("PB_OK"));
        PB_OK->setGeometry(QRect(390, 290, 75, 23));
        GV_Logo = new QGraphicsView(DyWindowAbout);
        GV_Logo->setObjectName(QStringLiteral("GV_Logo"));
        GV_Logo->setGeometry(QRect(0, 0, 480, 100));
        sizePolicy.setHeightForWidth(GV_Logo->sizePolicy().hasHeightForWidth());
        GV_Logo->setSizePolicy(sizePolicy);
        GV_Logo->setFrameShape(QFrame::NoFrame);
        GV_Logo->setFrameShadow(QFrame::Plain);
        PB_Log = new QPushButton(DyWindowAbout);
        PB_Log->setObjectName(QStringLiteral("PB_Log"));
        PB_Log->setGeometry(QRect(390, 110, 75, 23));

        retranslateUi(DyWindowAbout);

        QMetaObject::connectSlotsByName(DyWindowAbout);
    } // setupUi

    void retranslateUi(QWidget *DyWindowAbout)
    {
        DyWindowAbout->setWindowTitle(QApplication::translate("DyWindowAbout", "About", nullptr));
        label->setText(QApplication::translate("DyWindowAbout", "DianYing Font Atlas Generator", nullptr));
        label_2->setText(QApplication::translate("DyWindowAbout", "Version 2", nullptr));
        label_3->setText(QApplication::translate("DyWindowAbout", "This product reserved by GNU LGPL v3 license.", nullptr));
        label_4->setText(QApplication::translate("DyWindowAbout", "Used third-party libraries", nullptr));
        PB_OK->setText(QApplication::translate("DyWindowAbout", "OK", nullptr));
        PB_Log->setText(QApplication::translate("DyWindowAbout", "Update Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DyWindowAbout: public Ui_DyWindowAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DYWINDOWABOUT_H
