/********************************************************************************
** Form generated from reading UI file 'DyWindowAbout.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DYWINDOWABOUT_H
#define UI_DYWINDOWABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DyWindowAbout
{
public:

    void setupUi(QWidget *DyWindowAbout)
    {
        if (DyWindowAbout->objectName().isEmpty())
            DyWindowAbout->setObjectName(QStringLiteral("DyWindowAbout"));
        DyWindowAbout->resize(400, 300);

        retranslateUi(DyWindowAbout);

        QMetaObject::connectSlotsByName(DyWindowAbout);
    } // setupUi

    void retranslateUi(QWidget *DyWindowAbout)
    {
        DyWindowAbout->setWindowTitle(QApplication::translate("DyWindowAbout", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DyWindowAbout: public Ui_DyWindowAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DYWINDOWABOUT_H
