#include "friend.h"

Friend::Friend(QWidget *parent)
    : QWidget{parent}
{
    m_pShowMsgTE = new QTextEdit;
    m_pFriendlistWidget = new QListWidget;
    m_pInputMsgLE = new QLineEdit;

    m_pDelFriendPB = new QPushButton("Delete friend");
    m_pFlushFriendPB = new QPushButton("Refresh friend list");
    m_pShownOnineUsrPB = new QPushButton("Shown online friend");
    m_pSearchUsrPB = new QPushButton("Find friend");
    m_pMsgSendPB = new QPushButton("Send message");
    m_pPrivateChatPB = new QPushButton("Private chat");

    QVBoxLayout *pRightPBVBL = new QVBoxLayout;
    pRightPBVBL -> addWidget(m_pDelFriendPB);
    pRightPBVBL -> addWidget(m_pFlushFriendPB);
    pRightPBVBL -> addWidget(m_pShownOnineUsrPB);
    pRightPBVBL -> addWidget(m_pSearchUsrPB);

    QHBoxLayout *pTopHBL = new QHBoxLayout;
    pTopHBL -> addWidget(m_pShowMsgTE);
    pTopHBL -> addWidget(m_pFriendlistWidget);
    pTopHBL -> addLayout(pRightPBVBL);

    QHBoxLayout *pMsgHBL = new QHBoxLayout;
    pMsgHBL -> addWidget(m_pInputMsgLE);
    pMsgHBL -> addWidget(m_pMsgSendPB);

    m_pOnline = new Online;

    QVBoxLayout *pMain = new QVBoxLayout;
    pMain -> addLayout(pTopHBL);
    pMain -> addLayout(pMsgHBL);
    pMain -> addWidget(m_pOnline);
    m_pOnline->hide();

    setLayout(pMain);

    connect(m_pShownOnineUsrPB, SIGNAL(clicked(bool)), this, SLOT(showOnline()));
}

void Friend::showOnline()
{
    if (m_pOnline -> isHidden())
    {
        m_pOnline -> show();
    }
    else
    {
        m_pOnline -> hide();
    }
}
