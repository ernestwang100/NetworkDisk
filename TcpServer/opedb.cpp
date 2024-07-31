#include "opedb.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

OpeDB::OpeDB(QObject *parent)
    : QObject{parent}
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    QString dbPath = "/home/ernest/2024/summer/NetworkDisk/TcpServer/cloud.db"; // Use absolute path
    m_db.setDatabaseName(dbPath);
    if (m_db.open())
    {
        QSqlQuery query;
        if (query.exec("select * from userInfo"))
        {
            while (query.next())
            {
                QString data = QString("%1,%2,%3,%4").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString()).arg(query.value(3).toString());
                qDebug() << data;
                // ui -> textEdit -> append(data);
            }
        }
        else
        {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            QMessageBox::critical(NULL, "Query Execution Error", "Failed to execute query");
        }
    }
    else
    {
        qDebug() << "Failed to open database:" << m_db.lastError().text();
        QMessageBox::critical(NULL, "Database Error", "Failed to open database");
    }
}

OpeDB::~OpeDB()
{
    m_db.close();
}

bool OpeDB::handleReist(const char *name, const char *pwd)
{
    if (NULL == name || NULL == pwd)
    {
        return false;
    }
    QString data = QString("insert into userInfo(name, pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query;
    return query.exec(data);
}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if (NULL == name || NULL == pwd)
    {
        qDebug() << "name| pwd is NULL";
        return false;
    }
    QString data = QString("select * from userInfo where name = \'%1\' and pwd = \'%2\' and online = 0").arg(name).arg(pwd);
    qDebug() << data;
    QSqlQuery query;
    query.exec(data);
    if (query.next())
    {
        data = QString("update userInfo set online = 1 where name = \'%1\' and pwd = \'%2\'").arg(name).arg(pwd);
        qDebug() << data;
        QSqlQuery query;
        query.exec(data);
        return true;
    }
}

void OpeDB::handleOffline(const char *name)
{
    if (NULL == name)
    {
        qDebug() << "name is NULL";
        return;
    }
    QString data = QString("update userInfo set online = 0 where name = \'%1\'").arg(name);
    qDebug() << data;
    QSqlQuery query;
    query.exec(data);
}

QStringList OpeDB::handleAllOnline()
{
    QString data = QString("select name from userInfo where online = 1");
    // qDebug() << data;
    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();

    while (query.next())
    {
        result.append(query.value(0).toString());

    }
    return result;
}

int OpeDB::handleSearchUsr(const char *name)
{
    if (NULL == name)
    {
        return -1;
    }
    QString data = QString("select online from userInfo where name = \'%1\'").arg(name);
    // qDebug() << data;
    QSqlQuery query;
    query.exec(data);
    if (query.next())
    {
        qDebug() << query.value(0).toInt();
        return query.value(0).toInt();
    }
    else
    {
        return -1;
    }
}

int OpeDB::handleAddFriend(const char *pername, const char *name)
{
    if (NULL == pername || NULL == name)
    {
        return -1;
    }
    QString data = QString("select * from friend "
                           "where (id = (select id from userInfo where name = \'%1\') and "
                           "friendId = (select id from userInfo where name = \'%2\')) or "
                           "(id = (select id from userInfo where name = \'%3\') and "
                           "friendId = (select id from userInfo where name = \'%4\'))")
                       .arg(pername).arg(name).arg(name).arg(pername);

    qDebug() << data;
    QSqlQuery query;
    query.exec (data);
    if (query.next())
    {
        return 0; //friend already
    }
    else
    {
        data = QString("select online from userInfo where name = \'%1\'").arg(pername);
        qDebug() << data;
        QSqlQuery query;
        query.exec(data);
        if (query.next())
        {
            qDebug() << query.value(0).toInt();
            int ret = query.value(0).toInt();
            if (1 == ret)
            {
                return 1; //online
            }
            else if (0 == ret)
            {
                return 2; //offline
            }
        }
        else
        {
            return 3;
        }
    }

}

void OpeDB::handleAgreeAddFriend(const char *pername, const char *name)
{
    if (NULL == pername || NULL == name)
    {
        return;
    }
    QString data = QString("insert into friend(id, friendId) values ((select id from userInfo where name = \'%1\'), (select id from userInfo where name = \'%2\'))").arg(pername).arg(name);
    QSqlQuery query;
    query.exec(data);
}

QStringList OpeDB::handleFlushFriend(const char *name)
{
    QStringList strFriendList;
    strFriendList.clear();
    if (NULL == name)
    {
        return strFriendList;
    }
    QString strQuery = QString("select id from userInfo where name = \'%1\' and online = 1 ").arg(name);
    qDebug() << strQuery;
    QSqlQuery query;
    int iId = -1; // 请求方name对应的id
    query.exec(strQuery);
    if (query.next())
    {
        iId = query.value(0).toInt();
        qDebug() << "iId =" << iId;
    }


    // 查询好友信息表与用户信息表获取好友列表
    strQuery = QString("select name, online from userInfo "
                       "where id in "
                       "(select friendId from friend "
                       "where id = %1 "
                       "union "
                       "select id from friend "
                       "where friendId = %2)").arg(iId).arg(iId);
    query.exec(strQuery);
    while(query.next())
    {
        char friName[32];
        char friOnline[4];
        strncpy(friName, query.value(0).toString().toStdString().c_str(), 32);
        strncpy(friOnline, query.value(1).toString().toStdString().c_str(), 4);
        strFriendList.append(friName);
        strFriendList.append(friOnline);
        qDebug() << "好友信息 " << friName << " " << friOnline;
        qDebug() << strFriendList;
    }

    return strFriendList; // 返回查询到所有在线用户的姓名
}

bool OpeDB::handleDelFriend(const char *name, const char *friendName)
{
    if (NULL == name || NULL == friendName)
    {
        return false;
    }
    QString data = QString("delete from friend where id = (select id from userInfo where name =\'%1\') and friendId = (select id from userInfo where name =\'%2\')").arg(name).arg(friendName);
    QSqlQuery query;
    query.exec(data);

    data = QString("delete from friend where id = (select id from userInfo where name =\'%1\') and friendId = (select id from userInfo where name =\'%2\')").arg(friendName).arg(name);
    query.exec(data);

    return true;

}
