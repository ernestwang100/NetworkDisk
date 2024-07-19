#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    resize(500, 250);

    loadCnfig();

    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(showConnect()));
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(recvMsg()));

    // connect to server
    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadCnfig()
{
    QFile file(":/client.config");
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();
        file.close();
        strData.replace("\n", " ");

        QStringList strList = strData.split(" ");

        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug() << "ip: " << m_strIP << "port:" << m_usPort;

    }
    else
    {
        QMessageBox::critical(this, "open config", "open config fail");
    }
}

TcpClient &TcpClient::getinstance()
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}

void TcpClient::showConnect()
{
    QMessageBox::information(this, "connect to server", "connect sucessful");
}

void TcpClient::recvMsg()
{
    qDebug() << m_tcpSocket.bytesAvailable();
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen, sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu =  mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu+sizeof(uint), uiPDULen-sizeof(uint));
    qDebug() << pdu->uiMsgType;
    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        if (0 == strcmp(pdu->caData, REGIST_OK))
        {
            QMessageBox::information(this, "Register", REGIST_OK);
        }
        else if (0 == strcmp(pdu->caData, REGIST_FAILED))
        {
            QMessageBox::information(this, "Register", REGIST_FAILED);

        }
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        if (0 == strcmp(pdu->caData, LOGIN_OK))
        {
            QMessageBox::information(this, "Login", LOGIN_OK);
            OpeWidget::getInstance().show();
            hide();
        }
        else if (0 == strcmp(pdu->caData, LOGIN_FAILED))
        {
            QMessageBox::information(this, "Login", LOGIN_FAILED);

        }
    }

    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {
        OpeWidget::getInstance().getFriend() -> showAllOnlineUsr(pdu);
        break;
    }

    case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:
    {
        if (0 == strcmp(SEARCH_USR_NO, pdu -> caData))
        {
            QMessageBox::information(this, "Searching", QString("%1: not exist").arg(OpeWidget::getInstance().getFriend() -> m_strSearchName));
        }
        else if (0 == strcmp(SEARCH_USR_ONLINE, pdu -> caData))
        {
            QMessageBox::information(this, "Searching", QString("%1: online").arg(OpeWidget::getInstance().getFriend() -> m_strSearchName));

        }
        else if (0 == strcmp(SEARCH_USR_OFFLINE, pdu -> caData))
        {
            QMessageBox::information(this, "Searching", QString("%1: offline").arg(OpeWidget::getInstance().getFriend() -> m_strSearchName));

        }
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;

}
#if 0
void TcpClient::on_send_pb_clicked()
{
    QString strMsg = ui->name_le->text();
    if (!strMsg.isEmpty())
    {
        PDU *pdu = mkPDU(strMsg.size()+1);
        pdu->uiMsgType = 8888;
        memcpy(pdu->caMsg, strMsg.toStdString().c_str(), strMsg.size());
        qDebug() << (char*)(pdu->caMsg);
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this, "message sending", "message could not be null");
    }
}
#endif

void TcpClient::on_login_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if (!strName.isEmpty() && !strPwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "Login", "Login fail: username or password is null");

    }
}


void TcpClient::on_regist_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if (!strName.isEmpty() && !strPwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        strncpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "Sign up", "Sign up fail: username or password is null");

    }
}


void TcpClient::on_cancel_pb_clicked()
{

}

