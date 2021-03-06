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
    QPlainTextEdit *TV_FirstFilePath;
    QPushButton *BT_FirstFindFile;
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
    QPlainTextEdit *TV_SecondFilePath;
    QPushButton *BT_SecondFindFile;
    QLabel *label_5;
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
        DyFontAtlasGeneratorClass->resize(540, 380);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DyFontAtlasGeneratorClass->sizePolicy().hasHeightForWidth());
        DyFontAtlasGeneratorClass->setSizePolicy(sizePolicy);
        DyFontAtlasGeneratorClass->setMinimumSize(QSize(500, 0));
        DyFontAtlasGeneratorClass->setContextMenuPolicy(Qt::NoContextMenu);
        DyFontAtlasGeneratorClass->setWindowTitle(QStringLiteral("DianYing SDF FontAtlas generator (v190227)"));
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
        label->setGeometry(QRect(10, 10, 91, 21));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        TV_FirstFilePath = new QPlainTextEdit(centralWidget);
        TV_FirstFilePath->setObjectName(QStringLiteral("TV_FirstFilePath"));
        TV_FirstFilePath->setGeometry(QRect(110, 10, 331, 21));
        sizePolicy.setHeightForWidth(TV_FirstFilePath->sizePolicy().hasHeightForWidth());
        TV_FirstFilePath->setSizePolicy(sizePolicy);
        TV_FirstFilePath->setContextMenuPolicy(Qt::NoContextMenu);
        TV_FirstFilePath->setAcceptDrops(true);
#ifndef QT_NO_TOOLTIP
        TV_FirstFilePath->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        TV_FirstFilePath->setStatusTip(QStringLiteral(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        TV_FirstFilePath->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
        TV_FirstFilePath->setFrameShape(QFrame::StyledPanel);
        TV_FirstFilePath->setFrameShadow(QFrame::Plain);
        TV_FirstFilePath->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_FirstFilePath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_FirstFilePath->setDocumentTitle(QStringLiteral(""));
        TV_FirstFilePath->setUndoRedoEnabled(false);
        TV_FirstFilePath->setReadOnly(true);
        TV_FirstFilePath->setPlainText(QStringLiteral(""));
        TV_FirstFilePath->setPlaceholderText(QStringLiteral(""));
        BT_FirstFindFile = new QPushButton(centralWidget);
        BT_FirstFindFile->setObjectName(QStringLiteral("BT_FirstFindFile"));
        BT_FirstFindFile->setGeometry(QRect(450, 10, 75, 23));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 105, 541, 21));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 115, 501, 21));
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
        PG_Loading->setGeometry(QRect(0, 325, 541, 20));
        PG_Loading->setValue(0);
        PG_Loading->setTextVisible(false);
        CB_MapEnglish = new QCheckBox(centralWidget);
        CB_MapEnglish->setObjectName(QStringLiteral("CB_MapEnglish"));
        CB_MapEnglish->setEnabled(false);
        CB_MapEnglish->setGeometry(QRect(20, 145, 111, 21));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(-1, 204, 541, 21));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(0, 215, 541, 19));
        label_3->setMargin(0);
        label_3->setIndent(20);
        CB_OptionSeperate = new QCheckBox(centralWidget);
        CB_OptionSeperate->setObjectName(QStringLiteral("CB_OptionSeperate"));
        CB_OptionSeperate->setGeometry(QRect(20, 245, 271, 21));
        BT_Create = new QPushButton(centralWidget);
        BT_Create->setObjectName(QStringLiteral("BT_Create"));
        BT_Create->setGeometry(QRect(450, 285, 75, 23));
        CB_MapHangul = new QCheckBox(centralWidget);
        CB_MapHangul->setObjectName(QStringLiteral("CB_MapHangul"));
        CB_MapHangul->setEnabled(false);
        CB_MapHangul->setGeometry(QRect(160, 145, 71, 21));
        CB_OptionCompressJson = new QCheckBox(centralWidget);
        CB_OptionCompressJson->setObjectName(QStringLiteral("CB_OptionCompressJson"));
        CB_OptionCompressJson->setEnabled(false);
        CB_OptionCompressJson->setGeometry(QRect(20, 275, 311, 21));
        CB_MapKana = new QCheckBox(centralWidget);
        CB_MapKana->setObjectName(QStringLiteral("CB_MapKana"));
        CB_MapKana->setEnabled(false);
        CB_MapKana->setGeometry(QRect(250, 145, 141, 21));
        CB_MapCJKHanbun = new QCheckBox(centralWidget);
        CB_MapCJKHanbun->setObjectName(QStringLiteral("CB_MapCJKHanbun"));
        CB_MapCJKHanbun->setEnabled(false);
        CB_MapCJKHanbun->setGeometry(QRect(390, 145, 101, 21));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setEnabled(true);
        label_4->setGeometry(QRect(10, 80, 61, 21));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        TV_TextFilePath = new QPlainTextEdit(centralWidget);
        TV_TextFilePath->setObjectName(QStringLiteral("TV_TextFilePath"));
        TV_TextFilePath->setEnabled(true);
        TV_TextFilePath->setGeometry(QRect(110, 80, 331, 21));
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
        TV_TextFilePath->setPlainText(QStringLiteral(""));
        TV_TextFilePath->setPlaceholderText(QStringLiteral(""));
        BT_FindTextFile = new QPushButton(centralWidget);
        BT_FindTextFile->setObjectName(QStringLiteral("BT_FindTextFile"));
        BT_FindTextFile->setEnabled(true);
        BT_FindTextFile->setGeometry(QRect(450, 80, 75, 23));
        CB_MapAutomatic = new QCheckBox(centralWidget);
        CB_MapAutomatic->setObjectName(QStringLiteral("CB_MapAutomatic"));
        CB_MapAutomatic->setEnabled(false);
        CB_MapAutomatic->setGeometry(QRect(20, 175, 461, 21));
        TV_SecondFilePath = new QPlainTextEdit(centralWidget);
        TV_SecondFilePath->setObjectName(QStringLiteral("TV_SecondFilePath"));
        TV_SecondFilePath->setEnabled(false);
        TV_SecondFilePath->setGeometry(QRect(110, 40, 331, 21));
        sizePolicy.setHeightForWidth(TV_SecondFilePath->sizePolicy().hasHeightForWidth());
        TV_SecondFilePath->setSizePolicy(sizePolicy);
        TV_SecondFilePath->setContextMenuPolicy(Qt::NoContextMenu);
        TV_SecondFilePath->setAcceptDrops(true);
#ifndef QT_NO_TOOLTIP
        TV_SecondFilePath->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        TV_SecondFilePath->setStatusTip(QStringLiteral(""));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        TV_SecondFilePath->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
        TV_SecondFilePath->setFrameShape(QFrame::StyledPanel);
        TV_SecondFilePath->setFrameShadow(QFrame::Plain);
        TV_SecondFilePath->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_SecondFilePath->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TV_SecondFilePath->setDocumentTitle(QStringLiteral(""));
        TV_SecondFilePath->setUndoRedoEnabled(false);
        TV_SecondFilePath->setReadOnly(true);
        TV_SecondFilePath->setPlainText(QStringLiteral(""));
        TV_SecondFilePath->setPlaceholderText(QStringLiteral(""));
        BT_SecondFindFile = new QPushButton(centralWidget);
        BT_SecondFindFile->setObjectName(QStringLiteral("BT_SecondFindFile"));
        BT_SecondFindFile->setEnabled(false);
        BT_SecondFindFile->setGeometry(QRect(450, 40, 75, 23));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 40, 111, 21));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        DyFontAtlasGeneratorClass->setCentralWidget(centralWidget);
        MNB_Menu = new QMenuBar(DyFontAtlasGeneratorClass);
        MNB_Menu->setObjectName(QStringLiteral("MNB_Menu"));
        MNB_Menu->setGeometry(QRect(0, 0, 540, 21));
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
        label->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Primary Font", nullptr));
        BT_FirstFindFile->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Find", nullptr));
        label_2->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Create SDF Texture Atlas with...", nullptr));
        CB_MapEnglish->setText(QApplication::translate("DyFontAtlasGeneratorClass", "English + ASCII", nullptr));
        label_3->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Export option", nullptr));
        CB_OptionSeperate->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Export as separated setting and png files.", nullptr));
        BT_Create->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Create", nullptr));
        CB_MapHangul->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Hangul", nullptr));
        CB_OptionCompressJson->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Export with decompressed plain JSON setting file", nullptr));
        CB_MapKana->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Kana + Symbol", nullptr));
        CB_MapCJKHanbun->setText(QApplication::translate("DyFontAtlasGeneratorClass", "CJK Hanbun", nullptr));
        label_4->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Text File :", nullptr));
        BT_FindTextFile->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Find", nullptr));
        CB_MapAutomatic->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Automatically create textures of all font characters at text file.", nullptr));
        BT_SecondFindFile->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Find", nullptr));
        label_5->setText(QApplication::translate("DyFontAtlasGeneratorClass", "Secondary Font", nullptr));
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
