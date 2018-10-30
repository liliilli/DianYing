/********************************************************************************
** Form generated from reading UI file 'DyFontAtlasGenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DYFONTATLASGENERATOR_H
#define UI_DYFONTATLASGENERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DyFontAtlasGeneratorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DyFontAtlasGeneratorClass)
    {
        if (DyFontAtlasGeneratorClass->objectName().isEmpty())
            DyFontAtlasGeneratorClass->setObjectName(QStringLiteral("DyFontAtlasGeneratorClass"));
        DyFontAtlasGeneratorClass->resize(600, 400);
        menuBar = new QMenuBar(DyFontAtlasGeneratorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        DyFontAtlasGeneratorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DyFontAtlasGeneratorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DyFontAtlasGeneratorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(DyFontAtlasGeneratorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        DyFontAtlasGeneratorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(DyFontAtlasGeneratorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DyFontAtlasGeneratorClass->setStatusBar(statusBar);

        retranslateUi(DyFontAtlasGeneratorClass);

        QMetaObject::connectSlotsByName(DyFontAtlasGeneratorClass);
    } // setupUi

    void retranslateUi(QMainWindow *DyFontAtlasGeneratorClass)
    {
        DyFontAtlasGeneratorClass->setWindowTitle(QApplication::translate("DyFontAtlasGeneratorClass", "DyFontAtlasGenerator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DyFontAtlasGeneratorClass: public Ui_DyFontAtlasGeneratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DYFONTATLASGENERATOR_H
