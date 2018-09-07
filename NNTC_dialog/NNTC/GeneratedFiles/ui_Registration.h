/********************************************************************************
** Form generated from reading UI file 'Registration.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATION_H
#define UI_REGISTRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Registration
{
public:
    QTableView *TableUserInfo;
    QTableView *TableTrainInfo;
    QLabel *label;
    QPushButton *ButtonUserNew;
    QPushButton *ButtonUserModify;
    QPushButton *ButtonUserDel;
    QTextEdit *textEdit;
    QPushButton *ButtonUserSearch;
    QPushButton *ButtonTrainDel;
    QPushButton *ButtonTrainReport;
    QPushButton *ButtonTLC;
    QPushButton *ButtonTrainNew;

    void setupUi(QDialog *Registration)
    {
        if (Registration->objectName().isEmpty())
            Registration->setObjectName(QStringLiteral("Registration"));
        Registration->resize(1920, 1080);
        Registration->setContextMenuPolicy(Qt::NoContextMenu);
        TableUserInfo = new QTableView(Registration);
        TableUserInfo->setObjectName(QStringLiteral("TableUserInfo"));
        TableUserInfo->setGeometry(QRect(155, 285, 766, 490));
        TableUserInfo->setFocusPolicy(Qt::NoFocus);
        TableUserInfo->setContextMenuPolicy(Qt::NoContextMenu);
        TableTrainInfo = new QTableView(Registration);
        TableTrainInfo->setObjectName(QStringLiteral("TableTrainInfo"));
        TableTrainInfo->setGeometry(QRect(978, 285, 766, 490));
        TableTrainInfo->setFocusPolicy(Qt::NoFocus);
        TableTrainInfo->setContextMenuPolicy(Qt::NoContextMenu);
        label = new QLabel(Registration);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 1920, 1080));
        label->setContextMenuPolicy(Qt::NoContextMenu);
        label->setStyleSheet(QStringLiteral("background-image: url(:/NNTC/Resources/page2/21.jpg);"));
        ButtonUserNew = new QPushButton(Registration);
        ButtonUserNew->setObjectName(QStringLiteral("ButtonUserNew"));
        ButtonUserNew->setGeometry(QRect(155, 787, 123, 43));
        ButtonUserNew->setFocusPolicy(Qt::NoFocus);
        ButtonUserNew->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonUserModify = new QPushButton(Registration);
        ButtonUserModify->setObjectName(QStringLiteral("ButtonUserModify"));
        ButtonUserModify->setGeometry(QRect(288, 787, 123, 43));
        ButtonUserModify->setFocusPolicy(Qt::NoFocus);
        ButtonUserModify->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonUserDel = new QPushButton(Registration);
        ButtonUserDel->setObjectName(QStringLiteral("ButtonUserDel"));
        ButtonUserDel->setGeometry(QRect(421, 787, 123, 43));
        ButtonUserDel->setFocusPolicy(Qt::NoFocus);
        ButtonUserDel->setContextMenuPolicy(Qt::NoContextMenu);
        textEdit = new QTextEdit(Registration);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(554, 787, 233, 43));
        textEdit->setFocusPolicy(Qt::NoFocus);
        textEdit->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonUserSearch = new QPushButton(Registration);
        ButtonUserSearch->setObjectName(QStringLiteral("ButtonUserSearch"));
        ButtonUserSearch->setGeometry(QRect(797, 787, 123, 43));
        ButtonUserSearch->setFocusPolicy(Qt::NoFocus);
        ButtonUserSearch->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonTrainDel = new QPushButton(Registration);
        ButtonTrainDel->setObjectName(QStringLiteral("ButtonTrainDel"));
        ButtonTrainDel->setGeometry(QRect(1245, 787, 123, 43));
        ButtonTrainDel->setFocusPolicy(Qt::NoFocus);
        ButtonTrainDel->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonTrainReport = new QPushButton(Registration);
        ButtonTrainReport->setObjectName(QStringLiteral("ButtonTrainReport"));
        ButtonTrainReport->setGeometry(QRect(1112, 787, 123, 43));
        ButtonTrainReport->setFocusPolicy(Qt::NoFocus);
        ButtonTrainReport->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonTLC = new QPushButton(Registration);
        ButtonTLC->setObjectName(QStringLiteral("ButtonTLC"));
        ButtonTLC->setGeometry(QRect(1378, 787, 123, 43));
        ButtonTLC->setFocusPolicy(Qt::NoFocus);
        ButtonTLC->setContextMenuPolicy(Qt::NoContextMenu);
        ButtonTrainNew = new QPushButton(Registration);
        ButtonTrainNew->setObjectName(QStringLiteral("ButtonTrainNew"));
        ButtonTrainNew->setGeometry(QRect(979, 787, 123, 43));
        ButtonTrainNew->setFocusPolicy(Qt::NoFocus);
        ButtonTrainNew->setContextMenuPolicy(Qt::NoContextMenu);
        label->raise();
        TableUserInfo->raise();
        TableTrainInfo->raise();
        ButtonUserNew->raise();
        ButtonUserModify->raise();
        ButtonUserDel->raise();
        textEdit->raise();
        ButtonUserSearch->raise();
        ButtonTrainDel->raise();
        ButtonTrainReport->raise();
        ButtonTLC->raise();
        ButtonTrainNew->raise();

        retranslateUi(Registration);

        QMetaObject::connectSlotsByName(Registration);
    } // setupUi

    void retranslateUi(QDialog *Registration)
    {
        Registration->setWindowTitle(QString());
        label->setText(QString());
        ButtonUserNew->setText(QApplication::translate("Registration", "NewUser", nullptr));
        ButtonUserModify->setText(QApplication::translate("Registration", "UserModify", nullptr));
        ButtonUserDel->setText(QApplication::translate("Registration", "UserDel", nullptr));
        ButtonUserSearch->setText(QApplication::translate("Registration", "UserSearch", nullptr));
        ButtonTrainDel->setText(QApplication::translate("Registration", "TrainDel", nullptr));
        ButtonTrainReport->setText(QApplication::translate("Registration", "TrainReport", nullptr));
        ButtonTLC->setText(QApplication::translate("Registration", "TLC", nullptr));
        ButtonTrainNew->setText(QApplication::translate("Registration", "NewTrain", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Registration: public Ui_Registration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATION_H
