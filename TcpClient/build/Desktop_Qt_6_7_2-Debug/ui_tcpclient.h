/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *name_lab;
    QLineEdit *name_le;
    QHBoxLayout *horizontalLayout_2;
    QLabel *pwd_lab;
    QLineEdit *pwd_le;
    QPushButton *login_pb;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *regist_pb;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_pb;

    void setupUi(QWidget *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName("TcpClient");
        TcpClient->resize(800, 600);
        verticalLayout_2 = new QVBoxLayout(TcpClient);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        name_lab = new QLabel(TcpClient);
        name_lab->setObjectName("name_lab");
        QFont font;
        font.setPointSize(20);
        name_lab->setFont(font);

        horizontalLayout->addWidget(name_lab);

        name_le = new QLineEdit(TcpClient);
        name_le->setObjectName("name_le");
        name_le->setFont(font);

        horizontalLayout->addWidget(name_le);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pwd_lab = new QLabel(TcpClient);
        pwd_lab->setObjectName("pwd_lab");
        pwd_lab->setFont(font);

        horizontalLayout_2->addWidget(pwd_lab);

        pwd_le = new QLineEdit(TcpClient);
        pwd_le->setObjectName("pwd_le");
        pwd_le->setFont(font);
        pwd_le->setEchoMode(QLineEdit::EchoMode::Password);

        horizontalLayout_2->addWidget(pwd_le);


        verticalLayout->addLayout(horizontalLayout_2);

        login_pb = new QPushButton(TcpClient);
        login_pb->setObjectName("login_pb");
        login_pb->setFont(font);

        verticalLayout->addWidget(login_pb);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        regist_pb = new QPushButton(TcpClient);
        regist_pb->setObjectName("regist_pb");
        QFont font1;
        font1.setPointSize(16);
        regist_pb->setFont(font1);

        horizontalLayout_3->addWidget(regist_pb);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        cancel_pb = new QPushButton(TcpClient);
        cancel_pb->setObjectName("cancel_pb");
        cancel_pb->setFont(font1);

        horizontalLayout_3->addWidget(cancel_pb);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QWidget *TcpClient)
    {
        TcpClient->setWindowTitle(QCoreApplication::translate("TcpClient", "TcpClient", nullptr));
        name_lab->setText(QCoreApplication::translate("TcpClient", "Username:", nullptr));
        pwd_lab->setText(QCoreApplication::translate("TcpClient", "Password:", nullptr));
        login_pb->setText(QCoreApplication::translate("TcpClient", "Login", nullptr));
        regist_pb->setText(QCoreApplication::translate("TcpClient", "Register", nullptr));
        cancel_pb->setText(QCoreApplication::translate("TcpClient", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
