/********************************************************************************
** Form generated from reading UI file 'promotepawn.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGTEST_H
#define UI_DIALOGTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogTest
{
public:
    QPushButton *rook;
    QLabel *label;
    QPushButton *knight;
    QPushButton *bishop;
    QPushButton *queen;

    void setupUi(QDialog *DialogTest)
    {
        if (DialogTest->objectName().isEmpty())
            DialogTest->setObjectName(QString::fromUtf8("PromotePawnDialog"));
        DialogTest->resize(289, 81);
        rook = new QPushButton(DialogTest);
        rook->setObjectName(QString::fromUtf8("rook"));
        rook->setGeometry(QRect(10, 40, 61, 28));
        label = new QLabel(DialogTest);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 10, 81, 20));
        knight = new QPushButton(DialogTest);
        knight->setObjectName(QString::fromUtf8("knight"));
        knight->setGeometry(QRect(80, 40, 61, 28));
        bishop = new QPushButton(DialogTest);
        bishop->setObjectName(QString::fromUtf8("bishop"));
        bishop->setGeometry(QRect(150, 40, 61, 28));
        queen = new QPushButton(DialogTest);
        queen->setObjectName(QString::fromUtf8("queen"));
        queen->setGeometry(QRect(220, 40, 61, 28));

        retranslateUi(DialogTest);

        QMetaObject::connectSlotsByName(DialogTest);
    } // setupUi

    void retranslateUi(QDialog *DialogTest)
    {
        DialogTest->setWindowTitle(QCoreApplication::translate("PromotePawnDialog", "Dialog", nullptr));
        rook->setText(QCoreApplication::translate("PromotePawnDialog", "Rook", nullptr));
        label->setText(QCoreApplication::translate("PromotePawnDialog", "PROMOTE PAWN", nullptr));
        knight->setText(QCoreApplication::translate("PromotePawnDialog", "Knight", nullptr));
        bishop->setText(QCoreApplication::translate("PromotePawnDialog", "Bishop", nullptr));
        queen->setText(QCoreApplication::translate("PromotePawnDialog", "Queen", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogTest: public Ui_DialogTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGTEST_H
