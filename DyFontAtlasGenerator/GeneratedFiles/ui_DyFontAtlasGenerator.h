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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DyFontAtlasGeneratorClass
{
public:
    QAction *AC_ExitProgram;
    QAction *AC_About;
    QWidget *centralWidget;
    QLabel *label;
    QPlainTextEdit *TV_FilePath;
    QPushButton *BT_FindFile;
    QFrame *line;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QProgressBar *PG_Loading;
    QCheckBox *CB_MapEnglish;
    QFrame *line_2;
    QLabel *label_3;
    QCheckBox *CB_OptionSeperate;
    QPushButton *BT_Create;
    QCheckBox *CB_MapHangul;
    QCheckBox *CB_OptionCompressJson;
    QMenuBar *MNB_Menu;
    QMenu *MN_Main;
    QMenu *MN_Help;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DyFontAtlasGeneratorClass)
    {
        if (DyFontAtlasGeneratorClass->objectName().isEmpty())
            DyFontAtlasGeneratorClass->setObjectName(QStringLiteral("DyFontAtlasGeneratorClass"));
        DyFontAtlasGeneratorClass->setEnabled(true);
        DyFontAtlasGeneratorClass->resize(500, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DyFontAtlasGeneratorClass->sizePolicy().hasHeightForWidth());
        DyFontAtlasGeneratorClass->setSizePolicy(sizePolicy);
        DyFontAtlasGeneratorClass->setMinimumSize(QSize(500, 0));
        DyFontAtlasGeneratorClass->setContextMenuPolicy(Qt::NoContextMenu);
        DyFontAtlasGeneratorClass->setWindowTitle(QStringLiteral("DianYing SDF Font Atlas generator"));
#ifndef QT_NO_TOOLTIP
        DyFontAtlasGeneratorClass->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        DyFontAtlasGeneratorClass->setStatusTip(QStringLiteral(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        DyFontAtlasGeneratorClass->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
        DyFontAtlasGeneratorClass->setLayoutDirection(Qt::LeftToRight);
        DyFontAtlasGeneratorClass->setAnimated(true);
        DyFontAtlasGeneratorClass->setTabShape(QTabWidget::Rounded);
        AC_ExitProgram = new QAction(DyFontAtlasGeneratorClass);
        AC_ExitProgram->setObjectName(QStringLiteral("AC_ExitProgram"));
        AC_ExitProgram->setIconVisibleInMenu(false);
        AC_ExitProgram->setShortcutVisibleInContextMenu(false);
        AC_About = new QAction(DyFontAtlasGeneratorClass);
        AC_About->setObjectName(QStringLiteral("AC_About"));
        centralWidget = new QWidget(DyFontAtlasGeneratorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setEnabled(true);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 61, 21));
        label->setAlignment(Qt::AlignCenter);
        TV_FilePath = new QPlainTextEdit(centralWidget);
        TV_FilePath->setObjectName(QStringLiteral("TV_FilePath"));
        TV_FilePath->setGeometry(QRect(80, 10, 311, 21));
        sizePolicy.setHeightForWidth(TV_FilePath->sizePolicy().hasHeightForWidth());
        TV_FilePath->setSizePolicy(sizePolicy);
        TV_FilePath->setContextMenuPolicy(Qt::NoContextMenu);
        TV_FilePath->setAcceptDrops(true);
#ifndef QT_NO_TOOLTIP
        TV_FilePath->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        TV_FilePath->setStatusTip(QStringLiteral(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        TV_FilePath->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
        TV_FilePath->setFrameShape(QFrame::StyledPanel);
        TV_FilePath->setFrameShadow(QFrame::Plain);
        TV_FilePath->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_FilePath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_FilePath->setDocumentTitle(QStringLiteral(""));
        TV_FilePath->setUndoRedoEnabled(false);
        TV_FilePath->setReadOnly(true);
        TV_FilePath->setPlainText(QStringLiteral(""));
        TV_FilePath->setPlaceholderText(QStringLiteral(""));
        BT_FindFile = new QPushButton(centralWidget);
        BT_FindFile->setObjectName(QStringLiteral("BT_FindFile"));
        BT_FindFile->setGeometry(QRect(410, 10, 75, 23));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 30, 501, 21));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 40, 501, 21));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMargin(0);
        label_2->setIndent(20);

        verticalLayout->addWidget(label_2);

        PG_Loading = new QProgressBar(centralWidget);
        PG_Loading->setObjectName(QStringLiteral("PG_Loading"));
        PG_Loading->setGeometry(QRect(0, 250, 500, 20));
        PG_Loading->setValue(0);
        PG_Loading->setTextVisible(false);
        CB_MapEnglish = new QCheckBox(centralWidget);
        CB_MapEnglish->setObjectName(QStringLiteral("CB_MapEnglish"));
        CB_MapEnglish->setGeometry(QRect(20, 70, 91, 21));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(-1, 99, 501, 21));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 110, 499, 19));
        label_3->setMargin(0);
        label_3->setIndent(20);
        CB_OptionSeperate = new QCheckBox(centralWidget);
        CB_OptionSeperate->setObjectName(QStringLiteral("CB_OptionSeperate"));
        CB_OptionSeperate->setGeometry(QRect(20, 140, 271, 21));
        BT_Create = new QPushButton(centralWidget);
        BT_Create->setObjectName(QStringLiteral("BT_Create"));
        BT_Create->setGeometry(QRect(410, 210, 75, 23));
        CB_MapHangul = new QCheckBox(centralWidget);
        CB_MapHangul->setObjectName(QStringLiteral("CB_MapHangul"));
        CB_MapHangul->setGeometry(QRect(110, 70, 141, 21));
        CB_OptionCompressJson = new QCheckBox(centralWidget);
        CB_OptionCompressJson->setObjectName(QStringLiteral("CB_OptionCompressJson"));
        CB_OptionCompressJson->setGeometry(QRect(20, 170, 271, 21));
        DyFontAtlasGeneratorClass->setCentralWidget(centralWidget);
        MNB_Menu = new QMenuBar(DyFontAtlasGeneratorClass);
        MNB_Menu->setObjectName(QStringLiteral("MNB_Menu"));
        MNB_Menu->setGeometry(QRect(0, 0, 500, 21));
        MN_Main = new QMenu(MNB_Menu);
        MN_Main->setObjectName(QStringLiteral("MN_Main"));
        MN_Help = new QMenu(MNB_Menu);
        MN_Help->setObjectName(QStringLiteral("MN_Help"));
        DyFontAtlasGeneratorClass->setMenuBar(MNB_Menu);
        mainToolBar = new QToolBar(DyFontAtlasGeneratorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DyFontAtlasGeneratorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DyFontAtlasGeneratorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DyFontAtlasGeneratorClass->setStatusBar(statusBar);

        MNB_Menu->addAction(MN_Main->menuAction());
        MNB_Menu->addAction(MN_Help->menuAction());
        MN_Main->addAction(AC_ExitProgram);
        MN_Help->addAction(AC_About);

        retranslateUi(DyFontAtlasGeneratorClass);
        QObject::connect(AC_ExitProgram, SIGNAL(triggered()), DyFontAtlasGeneratorClass, SLOT(close()));

        QMetaObject::connectSlotsByName(DyFontAtlasGeneratorClass);
    } // setupUi

    void retranslateUi(QMainWindow *DyFontAtlasGeneratorClass)
    {
        AC_ExitProgram->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Exit program", nullptr));
        AC_About->setText(QApplication::translate("DyFontAtlasGeneratorClass", "About...", nullptr));
        label->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Font :", nullptr));
        BT_FindFile->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Find", nullptr));
        label_2->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Create SDF Atlas with...", nullptr));
        CB_MapEnglish->setText(QApplication::translate("DyFontAtlasGeneratorClass", "English", nullptr));
        label_3->setText(QApplication::translate("DyFontAtlasGeneratorClass", "And...", nullptr));
        CB_OptionSeperate->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Export as separated JSON and png files.", nullptr));
        BT_Create->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Create", nullptr));
        CB_MapHangul->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Hangul (Unicode 2.0)", nullptr));
        CB_OptionCompressJson->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Compress JSON String (Binary)", nullptr));
        MN_Main->setTitle(QApplication::translate("DyFontAtlasGeneratorClass", "Main", nullptr));
        MN_Help->setTitle(QApplication::translate("DyFontAtlasGeneratorClass", "Help", nullptr));
        Q_UNUSED(DyFontAtlasGeneratorClass);
    } // retranslateUi

};

namespace Ui {
    class DyFontAtlasGeneratorClass: public Ui_DyFontAtlasGeneratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DYFONTATLASGENERATOR_H
