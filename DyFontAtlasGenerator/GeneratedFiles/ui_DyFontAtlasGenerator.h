/********************************************************************************
** Form generated from reading UI file 'DyFontAtlasGenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
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
    QCheckBox *CB_MapKana;
    QCheckBox *CB_MapCJKHanbun;
    QLabel *label_4;
    QPlainTextEdit *TV_TextFilePath;
    QPushButton *BT_FindTextFile;
    QCheckBox *CB_MapAutomatic;
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
        DyFontAtlasGeneratorClass->resize(500, 350);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DyFontAtlasGeneratorClass->sizePolicy().hasHeightForWidth());
        DyFontAtlasGeneratorClass->setSizePolicy(sizePolicy);
        DyFontAtlasGeneratorClass->setMinimumSize(QSize(500, 0));
        DyFontAtlasGeneratorClass->setContextMenuPolicy(Qt::NoContextMenu);
        DyFontAtlasGeneratorClass->setWindowTitle(QStringLiteral("DianYing Font Atlas generator (v2)"));
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
        line->setGeometry(QRect(0, 70, 501, 21));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 80, 501, 21));
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
        PG_Loading->setGeometry(QRect(0, 290, 500, 20));
        PG_Loading->setValue(0);
        PG_Loading->setTextVisible(false);
        CB_MapEnglish = new QCheckBox(centralWidget);
        CB_MapEnglish->setObjectName(QStringLiteral("CB_MapEnglish"));
        CB_MapEnglish->setGeometry(QRect(20, 110, 111, 21));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(-1, 169, 501, 21));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 180, 499, 19));
        label_3->setMargin(0);
        label_3->setIndent(20);
        CB_OptionSeperate = new QCheckBox(centralWidget);
        CB_OptionSeperate->setObjectName(QStringLiteral("CB_OptionSeperate"));
        CB_OptionSeperate->setGeometry(QRect(20, 210, 271, 21));
        BT_Create = new QPushButton(centralWidget);
        BT_Create->setObjectName(QStringLiteral("BT_Create"));
        BT_Create->setGeometry(QRect(410, 250, 75, 23));
        CB_MapHangul = new QCheckBox(centralWidget);
        CB_MapHangul->setObjectName(QStringLiteral("CB_MapHangul"));
        CB_MapHangul->setGeometry(QRect(160, 110, 71, 21));
        CB_OptionCompressJson = new QCheckBox(centralWidget);
        CB_OptionCompressJson->setObjectName(QStringLiteral("CB_OptionCompressJson"));
        CB_OptionCompressJson->setEnabled(false);
        CB_OptionCompressJson->setGeometry(QRect(20, 240, 271, 21));
        CB_MapKana = new QCheckBox(centralWidget);
        CB_MapKana->setObjectName(QStringLiteral("CB_MapKana"));
        CB_MapKana->setGeometry(QRect(250, 110, 141, 21));
        CB_MapCJKHanbun = new QCheckBox(centralWidget);
        CB_MapCJKHanbun->setObjectName(QStringLiteral("CB_MapCJKHanbun"));
        CB_MapCJKHanbun->setEnabled(true);
        CB_MapCJKHanbun->setGeometry(QRect(390, 110, 101, 21));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setEnabled(false);
        label_4->setGeometry(QRect(10, 45, 61, 21));
        label_4->setAlignment(Qt::AlignCenter);
        TV_TextFilePath = new QPlainTextEdit(centralWidget);
        TV_TextFilePath->setObjectName(QStringLiteral("TV_TextFilePath"));
        TV_TextFilePath->setEnabled(false);
        TV_TextFilePath->setGeometry(QRect(80, 45, 311, 21));
        sizePolicy.setHeightForWidth(TV_TextFilePath->sizePolicy().hasHeightForWidth());
        TV_TextFilePath->setSizePolicy(sizePolicy);
        TV_TextFilePath->setContextMenuPolicy(Qt::NoContextMenu);
        TV_TextFilePath->setAcceptDrops(true);
#ifndef QT_NO_TOOLTIP
        TV_TextFilePath->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        TV_TextFilePath->setStatusTip(QStringLiteral(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        TV_TextFilePath->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
        TV_TextFilePath->setFrameShape(QFrame::StyledPanel);
        TV_TextFilePath->setFrameShadow(QFrame::Plain);
        TV_TextFilePath->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_TextFilePath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_TextFilePath->setDocumentTitle(QStringLiteral(""));
        TV_TextFilePath->setUndoRedoEnabled(false);
        TV_TextFilePath->setReadOnly(true);
        TV_TextFilePath->setPlainText(QStringLiteral("NOT AVAILABLE NOW..."));
        TV_TextFilePath->setPlaceholderText(QStringLiteral(""));
        BT_FindTextFile = new QPushButton(centralWidget);
        BT_FindTextFile->setObjectName(QStringLiteral("BT_FindTextFile"));
        BT_FindTextFile->setEnabled(false);
        BT_FindTextFile->setGeometry(QRect(410, 45, 75, 23));
        CB_MapAutomatic = new QCheckBox(centralWidget);
        CB_MapAutomatic->setObjectName(QStringLiteral("CB_MapAutomatic"));
        CB_MapAutomatic->setEnabled(false);
        CB_MapAutomatic->setGeometry(QRect(20, 140, 461, 21));
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
        label->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Font       :", nullptr));
        BT_FindFile->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Find", nullptr));
        label_2->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Create SDF Texture Atlas with...", nullptr));
        CB_MapEnglish->setText(QApplication::translate("DyFontAtlasGeneratorClass", "English + ASCII", nullptr));
        label_3->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Export option", nullptr));
        CB_OptionSeperate->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Export as separated information and png files.", nullptr));
        BT_Create->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Create", nullptr));
        CB_MapHangul->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Hangul", nullptr));
        CB_OptionCompressJson->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Compress JSON String (Binary)", nullptr));
        CB_MapKana->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Kana + Symbol", nullptr));
        CB_MapCJKHanbun->setText(QApplication::translate("DyFontAtlasGeneratorClass", "CJK Hanbun", nullptr));
        label_4->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Text File :", nullptr));
        BT_FindTextFile->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Find", nullptr));
        CB_MapAutomatic->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Automatically create textures of all font characters at text file.", nullptr));
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
