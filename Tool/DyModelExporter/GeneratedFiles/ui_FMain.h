/********************************************************************************
** Form generated from reading UI file 'FMain.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FMAIN_H
#define UI_FMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FMainClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FMainClass)
    {
        if (FMainClass->objectName().isEmpty())
            FMainClass->setObjectName(QStringLiteral("FMainClass"));
        FMainClass->resize(600, 400);
        menuBar = new QMenuBar(FMainClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FMainClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FMainClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FMainClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FMainClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FMainClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FMainClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FMainClass->setStatusBar(statusBar);

        retranslateUi(FMainClass);

        QMetaObject::connectSlotsByName(FMainClass);
    } // setupUi

    void retranslateUi(QMainWindow *FMainClass)
    {
        FMainClass->setWindowTitle(QApplication::translate("FMainClass", "FMain", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FMainClass: public Ui_FMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FMAIN_H
