#include "friend.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QInputDialog>
#include <QDebug>

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
    connect(m_pSearchUsrPB, SIGNAL(clicked(bool)), this, SLOT(searchUsr()));
}

void Friend::showAllOnlineUsr(PDU *pdu)
{
    if (NULL == pdu)
    {
        return;
    }
    m_pOnline -> showUsr(pdu);
}

void Friend::showOnline()
{
    if (m_pOnline -> isHidden())
    {
        m_pOnline -> show();

        PDU *pdu = mkPDU(0);
        pdu -> uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpClient::getinstance().getTcpSocket().write((char*)pdu, pdu -> uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        m_pOnline -> hide();
    }
}

void Friend::searchUsr()
{
    m_strSearchName  = QInputDialog::getText(this, "Search", "Username: ");
    if (!m_strSearchName.isEmpty())
    {
        qDebug() << m_strSearchName;
        PDU *pdu = mkPDU(0);
        std::string searchNameStdStr = m_strSearchName.toStdString();
        memcpy(pdu->caData, searchNameStdStr.c_str(), searchNameStdStr.size());
        pdu->caData[searchNameStdStr.size()] = '\0';  // Null-terminate the string in the PDU data
        pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_REQUEST;
        TcpClient::getinstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

