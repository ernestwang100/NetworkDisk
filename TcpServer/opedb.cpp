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
                QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
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
    qDebug() << data;
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
    qDebug() << data;
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
