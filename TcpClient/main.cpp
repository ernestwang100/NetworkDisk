#include "tcpclient.h"
#include <QApplication>
#include "opewidget.h"
#include "online.h"
#include "friend.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Friend w;
    // w.show();
    // Online w;
    // w.show();
    // TcpClient w;
    // w.show();
    OpeWidget w;
    w.show();
    return a.exec();
}
