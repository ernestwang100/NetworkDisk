#include "mytcpsocket.h"
#include <QDebug>
#include "mytcpserver.h"

MyTcpSocket::MyTcpSocket() {
    connect(this, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(this, SIGNAL(disconnected()), this, SLOT(clientOffline()));
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::recvMsg()
{
    qDebug() << this ->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen, sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu =  mkPDU(uiMsgLen);
    this->read((char*)pdu+sizeof(uint), uiPDULen-sizeof(uint));
    qDebug() << "Message Type:" << (ENUM_MSG_TYPE)pdu->uiMsgType;
    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        strncpy(caPwd, pdu->caData+32, 32);
        bool ret = OpeDB::getInstance().handleReist(caName, caPwd);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
            if (ret)
        {
            strcpy(respdu->caData, REGIST_OK);
        }
            else
        {
            strcpy(respdu->caData, REGIST_FAILED);

        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        strncpy(caPwd, pdu->caData+32, 32);
        bool ret = OpeDB::getInstance().handleLogin(caName, caPwd);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
        if (ret)
        {
            strcpy(respdu->caData, LOGIN_OK);
            m_strName = caName;
        }
        else
        {
            strcpy(respdu->caData, LOGIN_FAILED);

        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }

    case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:
    {
        QStringList ret = OpeDB::getInstance().handleAllOnline();
        uint uiMsgLen = ret.size() * 32;
        PDU *respdu = mkPDU(uiMsgLen);
        respdu -> uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
        for (int i = 0; i < ret.size(); i++)
        {
            memcpy((char*)(respdu -> caMsg) + i * 32,
                   ret.at(i).toStdString().c_str(),
                   ret.at(i).size());
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }

    case ENUM_MSG_TYPE_SEARCH_USR_REQUEST    :
    {
        int ret = OpeDB::getInstance().handleSearchUsr(pdu -> caData);
        qDebug() << ret;
        PDU *respdu = mkPDU(0);
        respdu -> uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_RESPOND;
        if (-1 == ret)
        {
            strcpy(respdu -> caData, SEARCH_USR_NO);
        }
        else if (1 == ret)
        {
            strcpy(respdu -> caData, SEARCH_USR_ONLINE);
        }
        else if (0 == ret)
        {
            strcpy(respdu -> caData, SEARCH_USR_OFFLINE);

        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        char caPerName[32] = {'\0'};
        char caName[32] = {'\0'};
        strncpy(caPerName, pdu->caData, 32);
        strncpy(caName, pdu->caData+32, 32);
        int ret = OpeDB::getInstance().handleAddFriend(caPerName, caName);
        qDebug() <<"ret =" << ret;
        PDU *respdu = NULL;
        if (-1 == ret)
        {
            respdu = mkPDU(0);
            respdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu -> caData, UNKOWN_ERROR);
            write((char*)respdu, respdu->uiPDULen);
            free(respdu);
            respdu = NULL;
        }
        else if (0 == ret)
        {
            respdu = mkPDU(0);
            respdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu -> caData, EXISTED_FRIEND);
            write((char*)respdu, respdu->uiPDULen);
            free(respdu);
            respdu = NULL;
        }
        else if (1 == ret)
        {
            MyTcpServer::getInstance().resend(caPerName, pdu);
        }
        else if (2 == ret)
        {
            respdu = mkPDU(0);
            respdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu -> caData, ADD_FRIEND_OFFLINE);
            write((char*)respdu, respdu->uiPDULen);
            free(respdu);
            respdu = NULL;
        }
        else if (3 == ret)
        {
            respdu = mkPDU(0);
            respdu -> uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu -> caData, ADD_FRIEND_NONEXIST);
            write((char*)respdu, respdu->uiPDULen);
            free(respdu);
            respdu = NULL;
        }
        break;
    }


    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
    {
        qDebug() << "flush friend request";
        char caName[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        qDebug() << caName;
        QStringList ret = OpeDB::getInstance().handleFlushFriend(caName);
        uint uiMsgLen = ret.size() * 32;
        PDU *respdu = mkPDU(uiMsgLen);
        respdu -> uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
        for (int i = 0; i < ret.size(); i++)
        {
            memcpy((char*)(respdu -> caMsg) + i * 32, ret.at(i).toStdString().c_str(), ret.at(i).size());
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }

    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        char caSelfName[32] = {'\0'};
        char caFriendName[32] = {'\0'};
        strncpy(caSelfName, pdu->caData, 32);
        strncpy(caFriendName, pdu->caData+32, 32);
        OpeDB::getInstance().handleDelFriend(caSelfName, caFriendName);
        PDU *respdu = mkPDU(0);
        respdu -> uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
        strcpy(respdu -> caData, DEL_FRIEND_OK);
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;

        MyTcpServer::getInstance().resend(caFriendName, pdu);
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;

    // qDebug() << caName << caPwd << pdu->uiMsgType;
}

void MyTcpSocket::clientOffline()
{
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this);
}
