/********************************************************************************
** Form generated from reading UI file 'cpu.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPU_H
#define UI_CPU_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CPUView
{
public:
    QAction *actionPause;
    QAction *actionStep;
    QAction *actionNext;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QTableView *disassembly;
    QFormLayout *formLayout;
    QLabel *accumulatorLabel;
    QLineEdit *accumulatorLineEdit;
    QLabel *programBankRegisterLabel;
    QLineEdit *programBankRegisterLineEdit;
    QLabel *programCounterLabel;
    QLineEdit *programCounterLineEdit;
    QLabel *directBankLabel;
    QLineEdit *directBankLineEdit;
    QLabel *directPageLabel;
    QLineEdit *directPageLineEdit;
    QLabel *stackPointerLabel;
    QLineEdit *stackPointerLineEdit;
    QLabel *xIndexLabel;
    QLineEdit *xIndexLineEdit;
    QLabel *yIndexLabel;
    QLineEdit *yIndexLineEdit;
    QLabel *flagsLabel;
    QLineEdit *flagsLineEdit;
    QLabel *emulationModeLabel;
    QCheckBox *emulationModeCheckBox;
    QGridLayout *gridLayout;
    QPushButton *clear;
    QLabel *loggerLabel;
    QTextBrowser *logger;
    QToolBar *toolBar;

    void setupUi(QMainWindow *CPUView)
    {
        if (CPUView->objectName().isEmpty())
            CPUView->setObjectName(QString::fromUtf8("CPUView"));
        CPUView->resize(971, 709);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        CPUView->setWindowIcon(icon);
        CPUView->setAutoFillBackground(false);
        actionPause = new QAction(CPUView);
        actionPause->setObjectName(QString::fromUtf8("actionPause"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/icons/play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon1);
        actionStep = new QAction(CPUView);
        actionStep->setObjectName(QString::fromUtf8("actionStep"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/icons/step.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStep->setIcon(icon2);
        actionNext = new QAction(CPUView);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/icons/continue.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon3);
        centralwidget = new QWidget(CPUView);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        disassembly = new QTableView(centralwidget);
        disassembly->setObjectName(QString::fromUtf8("disassembly"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(disassembly->sizePolicy().hasHeightForWidth());
        disassembly->setSizePolicy(sizePolicy);
        disassembly->setStyleSheet(QString::fromUtf8(""));
        disassembly->setSelectionMode(QAbstractItemView::ExtendedSelection);
        disassembly->setSelectionBehavior(QAbstractItemView::SelectRows);
        disassembly->setShowGrid(false);
        disassembly->setGridStyle(Qt::NoPen);
        disassembly->horizontalHeader()->setVisible(false);
        disassembly->horizontalHeader()->setHighlightSections(false);

        gridLayout_2->addWidget(disassembly, 0, 0, 2, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        accumulatorLabel = new QLabel(centralwidget);
        accumulatorLabel->setObjectName(QString::fromUtf8("accumulatorLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, accumulatorLabel);

        accumulatorLineEdit = new QLineEdit(centralwidget);
        accumulatorLineEdit->setObjectName(QString::fromUtf8("accumulatorLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, accumulatorLineEdit);

        programBankRegisterLabel = new QLabel(centralwidget);
        programBankRegisterLabel->setObjectName(QString::fromUtf8("programBankRegisterLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, programBankRegisterLabel);

        programBankRegisterLineEdit = new QLineEdit(centralwidget);
        programBankRegisterLineEdit->setObjectName(QString::fromUtf8("programBankRegisterLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, programBankRegisterLineEdit);

        programCounterLabel = new QLabel(centralwidget);
        programCounterLabel->setObjectName(QString::fromUtf8("programCounterLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, programCounterLabel);

        programCounterLineEdit = new QLineEdit(centralwidget);
        programCounterLineEdit->setObjectName(QString::fromUtf8("programCounterLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, programCounterLineEdit);

        directBankLabel = new QLabel(centralwidget);
        directBankLabel->setObjectName(QString::fromUtf8("directBankLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, directBankLabel);

        directBankLineEdit = new QLineEdit(centralwidget);
        directBankLineEdit->setObjectName(QString::fromUtf8("directBankLineEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, directBankLineEdit);

        directPageLabel = new QLabel(centralwidget);
        directPageLabel->setObjectName(QString::fromUtf8("directPageLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, directPageLabel);

        directPageLineEdit = new QLineEdit(centralwidget);
        directPageLineEdit->setObjectName(QString::fromUtf8("directPageLineEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, directPageLineEdit);

        stackPointerLabel = new QLabel(centralwidget);
        stackPointerLabel->setObjectName(QString::fromUtf8("stackPointerLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, stackPointerLabel);

        stackPointerLineEdit = new QLineEdit(centralwidget);
        stackPointerLineEdit->setObjectName(QString::fromUtf8("stackPointerLineEdit"));

        formLayout->setWidget(5, QFormLayout::FieldRole, stackPointerLineEdit);

        xIndexLabel = new QLabel(centralwidget);
        xIndexLabel->setObjectName(QString::fromUtf8("xIndexLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, xIndexLabel);

        xIndexLineEdit = new QLineEdit(centralwidget);
        xIndexLineEdit->setObjectName(QString::fromUtf8("xIndexLineEdit"));

        formLayout->setWidget(6, QFormLayout::FieldRole, xIndexLineEdit);

        yIndexLabel = new QLabel(centralwidget);
        yIndexLabel->setObjectName(QString::fromUtf8("yIndexLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, yIndexLabel);

        yIndexLineEdit = new QLineEdit(centralwidget);
        yIndexLineEdit->setObjectName(QString::fromUtf8("yIndexLineEdit"));

        formLayout->setWidget(7, QFormLayout::FieldRole, yIndexLineEdit);

        flagsLabel = new QLabel(centralwidget);
        flagsLabel->setObjectName(QString::fromUtf8("flagsLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, flagsLabel);

        flagsLineEdit = new QLineEdit(centralwidget);
        flagsLineEdit->setObjectName(QString::fromUtf8("flagsLineEdit"));

        formLayout->setWidget(8, QFormLayout::FieldRole, flagsLineEdit);

        emulationModeLabel = new QLabel(centralwidget);
        emulationModeLabel->setObjectName(QString::fromUtf8("emulationModeLabel"));

        formLayout->setWidget(9, QFormLayout::LabelRole, emulationModeLabel);

        emulationModeCheckBox = new QCheckBox(centralwidget);
        emulationModeCheckBox->setObjectName(QString::fromUtf8("emulationModeCheckBox"));
        emulationModeCheckBox->setLayoutDirection(Qt::RightToLeft);
        emulationModeCheckBox->setCheckable(true);

        formLayout->setWidget(9, QFormLayout::FieldRole, emulationModeCheckBox);


        gridLayout_2->addLayout(formLayout, 0, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        clear = new QPushButton(centralwidget);
        clear->setObjectName(QString::fromUtf8("clear"));

        gridLayout->addWidget(clear, 2, 0, 1, 1);

        loggerLabel = new QLabel(centralwidget);
        loggerLabel->setObjectName(QString::fromUtf8("loggerLabel"));
        loggerLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(loggerLabel, 0, 0, 1, 1);

        logger = new QTextBrowser(centralwidget);
        logger->setObjectName(QString::fromUtf8("logger"));

        gridLayout->addWidget(logger, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 1, 1, 1);

        CPUView->setCentralWidget(centralwidget);
        toolBar = new QToolBar(CPUView);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMinimumSize(QSize(0, 0));
        toolBar->setMovable(false);
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setFloatable(true);
        CPUView->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionPause);
        toolBar->addAction(actionNext);
        toolBar->addAction(actionStep);

        retranslateUi(CPUView);

        QMetaObject::connectSlotsByName(CPUView);
    } // setupUi

    void retranslateUi(QMainWindow *CPUView)
    {
        CPUView->setWindowTitle(QCoreApplication::translate("CPUView", "CPU's Debugger", nullptr));
        actionPause->setText(QCoreApplication::translate("CPUView", "Continue", nullptr));
#if QT_CONFIG(tooltip)
        actionPause->setToolTip(QCoreApplication::translate("CPUView", "Pause or Resume instruction execution.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionPause->setShortcut(QCoreApplication::translate("CPUView", "C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStep->setText(QCoreApplication::translate("CPUView", "Step", nullptr));
#if QT_CONFIG(tooltip)
        actionStep->setToolTip(QCoreApplication::translate("CPUView", "Execute a single instruction", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionStep->setShortcut(QCoreApplication::translate("CPUView", "S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionNext->setText(QCoreApplication::translate("CPUView", "Next", nullptr));
#if QT_CONFIG(tooltip)
        actionNext->setToolTip(QCoreApplication::translate("CPUView", "Continue execution to the next line.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionNext->setShortcut(QCoreApplication::translate("CPUView", "N", nullptr));
#endif // QT_CONFIG(shortcut)
        accumulatorLabel->setText(QCoreApplication::translate("CPUView", "Accumulator", nullptr));
        accumulatorLineEdit->setText(QString());
        programBankRegisterLabel->setText(QCoreApplication::translate("CPUView", "Program Bank", nullptr));
        programCounterLabel->setText(QCoreApplication::translate("CPUView", "Program Counter", nullptr));
        directBankLabel->setText(QCoreApplication::translate("CPUView", "Direct Bank", nullptr));
        directPageLabel->setText(QCoreApplication::translate("CPUView", "Direct Page", nullptr));
        stackPointerLabel->setText(QCoreApplication::translate("CPUView", "Stack Pointer", nullptr));
        xIndexLabel->setText(QCoreApplication::translate("CPUView", "X Index", nullptr));
        yIndexLabel->setText(QCoreApplication::translate("CPUView", "Y Index", nullptr));
        flagsLabel->setText(QCoreApplication::translate("CPUView", "Flags", nullptr));
        emulationModeLabel->setText(QCoreApplication::translate("CPUView", "Emulation mode", nullptr));
        clear->setText(QCoreApplication::translate("CPUView", "Clear History", nullptr));
        loggerLabel->setText(QCoreApplication::translate("CPUView", "Instructions History", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("CPUView", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CPUView: public Ui_CPUView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPU_H
