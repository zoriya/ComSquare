/********************************************************************************
** Form generated from reading UI file 'cpu.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPU_H
#define UI_CPU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
//efef
QT_BEGIN_NAMESPACE

class Ui_CPUView
{
public:
    QAction *actionPause;
    QAction *actionStep;
    QAction *actionNext;
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QTableView *disassembly;
    QGridLayout *gridLayout_2;
    QTableView *stackView;
    QLabel *label;
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
    QLabel *emulationModeLabel;
    QCheckBox *emulationModeCheckBox;
    QGridLayout *gridLayout;
    QLabel *loggerLabel;
    QPushButton *clear;
    QTableView *history;
    QGroupBox *formGroupBox;
    QFormLayout *formLayout_2;
    QLabel *negativeLabel;
    QCheckBox *mCheckbox;
    QLabel *zeroLabel;
    QCheckBox *xCheckbox;
    QLabel *carryLabel;
    QCheckBox *iCheckbox;
    QLabel *Overflow;
    QCheckBox *vCheckbox;
    QLabel *decimalLabel;
    QCheckBox *dCheckbox;
    QLabel *memoryAccumulatorSelectLabel;
    QCheckBox *cCheckbox;
    QLabel *indeXSelectLabel;
    QLabel *irqDisableLabel;
    QCheckBox *nCheckbox;
    QCheckBox *zCheckbox;
    QLabel *breakBLabel;
    QCheckBox *bCheckbox;
    QToolBar *toolBar;

    void setupUi(QMainWindow *CPUView)
    {
        if (CPUView->objectName().isEmpty())
            CPUView->setObjectName(QStringLiteral("CPUView"));
        CPUView->resize(1058, 673);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        CPUView->setWindowIcon(icon);
        CPUView->setAutoFillBackground(false);
        actionPause = new QAction(CPUView);
        actionPause->setObjectName(QStringLiteral("actionPause"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resources/icons/play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause->setIcon(icon1);
        actionStep = new QAction(CPUView);
        actionStep->setObjectName(QStringLiteral("actionStep"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/resources/icons/step.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStep->setIcon(icon2);
        actionNext = new QAction(CPUView);
        actionNext->setObjectName(QStringLiteral("actionNext"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/resources/icons/continue.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon3);
        centralwidget = new QWidget(CPUView);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        disassembly = new QTableView(centralwidget);
        disassembly->setObjectName(QStringLiteral("disassembly"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(disassembly->sizePolicy().hasHeightForWidth());
        disassembly->setSizePolicy(sizePolicy);
        disassembly->setStyleSheet(QStringLiteral(""));
        disassembly->setSelectionMode(QAbstractItemView::ExtendedSelection);
        disassembly->setSelectionBehavior(QAbstractItemView::SelectRows);
        disassembly->setShowGrid(false);
        disassembly->setGridStyle(Qt::NoPen);
        disassembly->horizontalHeader()->setVisible(false);
        disassembly->horizontalHeader()->setHighlightSections(false);

        gridLayout_3->addWidget(disassembly, 0, 0, 3, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        stackView = new QTableView(centralwidget);
        stackView->setObjectName(QStringLiteral("stackView"));
        stackView->horizontalHeader()->setVisible(false);

        gridLayout_2->addWidget(stackView, 1, 0, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 1, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        accumulatorLabel = new QLabel(centralwidget);
        accumulatorLabel->setObjectName(QStringLiteral("accumulatorLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, accumulatorLabel);

        accumulatorLineEdit = new QLineEdit(centralwidget);
        accumulatorLineEdit->setObjectName(QStringLiteral("accumulatorLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, accumulatorLineEdit);

        programBankRegisterLabel = new QLabel(centralwidget);
        programBankRegisterLabel->setObjectName(QStringLiteral("programBankRegisterLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, programBankRegisterLabel);

        programBankRegisterLineEdit = new QLineEdit(centralwidget);
        programBankRegisterLineEdit->setObjectName(QStringLiteral("programBankRegisterLineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, programBankRegisterLineEdit);

        programCounterLabel = new QLabel(centralwidget);
        programCounterLabel->setObjectName(QStringLiteral("programCounterLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, programCounterLabel);

        programCounterLineEdit = new QLineEdit(centralwidget);
        programCounterLineEdit->setObjectName(QStringLiteral("programCounterLineEdit"));

        formLayout->setWidget(2, QFormLayout::FieldRole, programCounterLineEdit);

        directBankLabel = new QLabel(centralwidget);
        directBankLabel->setObjectName(QStringLiteral("directBankLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, directBankLabel);

        directBankLineEdit = new QLineEdit(centralwidget);
        directBankLineEdit->setObjectName(QStringLiteral("directBankLineEdit"));

        formLayout->setWidget(3, QFormLayout::FieldRole, directBankLineEdit);

        directPageLabel = new QLabel(centralwidget);
        directPageLabel->setObjectName(QStringLiteral("directPageLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, directPageLabel);

        directPageLineEdit = new QLineEdit(centralwidget);
        directPageLineEdit->setObjectName(QStringLiteral("directPageLineEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, directPageLineEdit);

        stackPointerLabel = new QLabel(centralwidget);
        stackPointerLabel->setObjectName(QStringLiteral("stackPointerLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, stackPointerLabel);

        stackPointerLineEdit = new QLineEdit(centralwidget);
        stackPointerLineEdit->setObjectName(QStringLiteral("stackPointerLineEdit"));

        formLayout->setWidget(5, QFormLayout::FieldRole, stackPointerLineEdit);

        xIndexLabel = new QLabel(centralwidget);
        xIndexLabel->setObjectName(QStringLiteral("xIndexLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, xIndexLabel);

        xIndexLineEdit = new QLineEdit(centralwidget);
        xIndexLineEdit->setObjectName(QStringLiteral("xIndexLineEdit"));

        formLayout->setWidget(6, QFormLayout::FieldRole, xIndexLineEdit);

        yIndexLabel = new QLabel(centralwidget);
        yIndexLabel->setObjectName(QStringLiteral("yIndexLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, yIndexLabel);

        yIndexLineEdit = new QLineEdit(centralwidget);
        yIndexLineEdit->setObjectName(QStringLiteral("yIndexLineEdit"));

        formLayout->setWidget(7, QFormLayout::FieldRole, yIndexLineEdit);

        emulationModeLabel = new QLabel(centralwidget);
        emulationModeLabel->setObjectName(QStringLiteral("emulationModeLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, emulationModeLabel);

        emulationModeCheckBox = new QCheckBox(centralwidget);
        emulationModeCheckBox->setObjectName(QStringLiteral("emulationModeCheckBox"));
        emulationModeCheckBox->setLayoutDirection(Qt::RightToLeft);
        emulationModeCheckBox->setCheckable(true);

        formLayout->setWidget(8, QFormLayout::FieldRole, emulationModeCheckBox);


        gridLayout_3->addLayout(formLayout, 0, 2, 2, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        loggerLabel = new QLabel(centralwidget);
        loggerLabel->setObjectName(QStringLiteral("loggerLabel"));
        loggerLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(loggerLabel, 0, 0, 1, 1);

        clear = new QPushButton(centralwidget);
        clear->setObjectName(QStringLiteral("clear"));

        gridLayout->addWidget(clear, 2, 0, 1, 1);

        history = new QTableView(centralwidget);
        history->setObjectName(QStringLiteral("history"));

        gridLayout->addWidget(history, 1, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 1, 2, 1);

        formGroupBox = new QGroupBox(centralwidget);
        formGroupBox->setObjectName(QStringLiteral("formGroupBox"));
        formGroupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formGroupBox->setFlat(false);
        formGroupBox->setCheckable(false);
        formLayout_2 = new QFormLayout(formGroupBox);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        formLayout_2->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_2->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        formLayout_2->setContentsMargins(26, 7, 0, -1);
        negativeLabel = new QLabel(formGroupBox);
        negativeLabel->setObjectName(QStringLiteral("negativeLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, negativeLabel);

        mCheckbox = new QCheckBox(formGroupBox);
        mCheckbox->setObjectName(QStringLiteral("mCheckbox"));
        mCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, mCheckbox);

        zeroLabel = new QLabel(formGroupBox);
        zeroLabel->setObjectName(QStringLiteral("zeroLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, zeroLabel);

        xCheckbox = new QCheckBox(formGroupBox);
        xCheckbox->setObjectName(QStringLiteral("xCheckbox"));
        xCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, xCheckbox);

        carryLabel = new QLabel(formGroupBox);
        carryLabel->setObjectName(QStringLiteral("carryLabel"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, carryLabel);

        iCheckbox = new QCheckBox(formGroupBox);
        iCheckbox->setObjectName(QStringLiteral("iCheckbox"));
        iCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, iCheckbox);

        Overflow = new QLabel(formGroupBox);
        Overflow->setObjectName(QStringLiteral("Overflow"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, Overflow);

        vCheckbox = new QCheckBox(formGroupBox);
        vCheckbox->setObjectName(QStringLiteral("vCheckbox"));
        vCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, vCheckbox);

        decimalLabel = new QLabel(formGroupBox);
        decimalLabel->setObjectName(QStringLiteral("decimalLabel"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, decimalLabel);

        dCheckbox = new QCheckBox(formGroupBox);
        dCheckbox->setObjectName(QStringLiteral("dCheckbox"));
        dCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(5, QFormLayout::FieldRole, dCheckbox);

        memoryAccumulatorSelectLabel = new QLabel(formGroupBox);
        memoryAccumulatorSelectLabel->setObjectName(QStringLiteral("memoryAccumulatorSelectLabel"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, memoryAccumulatorSelectLabel);

        cCheckbox = new QCheckBox(formGroupBox);
        cCheckbox->setObjectName(QStringLiteral("cCheckbox"));
        cCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(6, QFormLayout::FieldRole, cCheckbox);

        indeXSelectLabel = new QLabel(formGroupBox);
        indeXSelectLabel->setObjectName(QStringLiteral("indeXSelectLabel"));

        formLayout_2->setWidget(7, QFormLayout::LabelRole, indeXSelectLabel);

        irqDisableLabel = new QLabel(formGroupBox);
        irqDisableLabel->setObjectName(QStringLiteral("irqDisableLabel"));

        formLayout_2->setWidget(8, QFormLayout::LabelRole, irqDisableLabel);

        nCheckbox = new QCheckBox(formGroupBox);
        nCheckbox->setObjectName(QStringLiteral("nCheckbox"));
        nCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(8, QFormLayout::FieldRole, nCheckbox);

        zCheckbox = new QCheckBox(formGroupBox);
        zCheckbox->setObjectName(QStringLiteral("zCheckbox"));
        zCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(7, QFormLayout::FieldRole, zCheckbox);

        breakBLabel = new QLabel(formGroupBox);
        breakBLabel->setObjectName(QStringLiteral("breakBLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, breakBLabel);

        bCheckbox = new QCheckBox(formGroupBox);
        bCheckbox->setObjectName(QStringLiteral("bCheckbox"));
        bCheckbox->setLayoutDirection(Qt::RightToLeft);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, bCheckbox);


        gridLayout_3->addWidget(formGroupBox, 2, 2, 1, 1);

        CPUView->setCentralWidget(centralwidget);
        toolBar = new QToolBar(CPUView);
        toolBar->setObjectName(QStringLiteral("toolBar"));
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
        CPUView->setWindowTitle(QApplication::translate("CPUView", "CPU's Debugger", Q_NULLPTR));
        actionPause->setText(QApplication::translate("CPUView", "Continue", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionPause->setToolTip(QApplication::translate("CPUView", "Pause or Resume instruction execution.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionPause->setShortcut(QApplication::translate("CPUView", "C", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionStep->setText(QApplication::translate("CPUView", "Step", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionStep->setToolTip(QApplication::translate("CPUView", "Execute a single instruction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionStep->setShortcut(QApplication::translate("CPUView", "S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionNext->setText(QApplication::translate("CPUView", "Next", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("CPUView", "Continue execution to the next line.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNext->setShortcut(QApplication::translate("CPUView", "N", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        label->setText(QApplication::translate("CPUView", "Stack Viewer", Q_NULLPTR));
        accumulatorLabel->setText(QApplication::translate("CPUView", "Accumulator", Q_NULLPTR));
        accumulatorLineEdit->setText(QString());
        programBankRegisterLabel->setText(QApplication::translate("CPUView", "Program Bank", Q_NULLPTR));
        programCounterLabel->setText(QApplication::translate("CPUView", "Program Counter", Q_NULLPTR));
        directBankLabel->setText(QApplication::translate("CPUView", "Direct Bank", Q_NULLPTR));
        directPageLabel->setText(QApplication::translate("CPUView", "Direct Page", Q_NULLPTR));
        stackPointerLabel->setText(QApplication::translate("CPUView", "Stack Pointer", Q_NULLPTR));
        xIndexLabel->setText(QApplication::translate("CPUView", "X Index", Q_NULLPTR));
        yIndexLabel->setText(QApplication::translate("CPUView", "Y Index", Q_NULLPTR));
        emulationModeLabel->setText(QApplication::translate("CPUView", "Emulation mode", Q_NULLPTR));
        loggerLabel->setText(QApplication::translate("CPUView", "Instructions History", Q_NULLPTR));
        clear->setText(QApplication::translate("CPUView", "Clear History", Q_NULLPTR));
        formGroupBox->setTitle(QApplication::translate("CPUView", "Flags", Q_NULLPTR));
        negativeLabel->setText(QApplication::translate("CPUView", "Memory Select (M)", Q_NULLPTR));
        zeroLabel->setText(QApplication::translate("CPUView", "Index Select (X)", Q_NULLPTR));
        carryLabel->setText(QApplication::translate("CPUView", "Interupt Request Disable (I)", Q_NULLPTR));
        Overflow->setText(QApplication::translate("CPUView", "Overflow (V)", Q_NULLPTR));
        decimalLabel->setText(QApplication::translate("CPUView", "Decimal (D)", Q_NULLPTR));
        memoryAccumulatorSelectLabel->setText(QApplication::translate("CPUView", "Carry (C)", Q_NULLPTR));
        indeXSelectLabel->setText(QApplication::translate("CPUView", "Zero (Z)", Q_NULLPTR));
        irqDisableLabel->setText(QApplication::translate("CPUView", "Negative (N)", Q_NULLPTR));
        breakBLabel->setText(QApplication::translate("CPUView", "Break (B)", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("CPUView", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CPUView: public Ui_CPUView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPU_H
