#include "mytcpserver.h"
#include <QDebug>

MyTcpServer::MyTcpServer() {}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "New client connected";
    MyTcpSocket *pTcpSocket = new MyTcpSocket;
    if (!pTcpSocket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Failed to set socket descriptor";
        delete pTcpSocket;
        return;
    }
    m_tcpSocketList.append(pTcpSocket);

    connect(pTcpSocket, &MyTcpSocket::offline, this, &MyTcpServer::deleteSocket);
}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    QList<MyTcpSocket*>::iterator iter = m_tcpSocketList.begin();
    while (iter != m_tcpSocketList.end())
    {
        if (mysocket == *iter)
        {
            delete *iter;
            iter = m_tcpSocketList.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }

    for (MyTcpSocket* socket : m_tcpSocketList)
    {
        qDebug() << socket->getName();
    }
}
