#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QtSql/QSqlDatabase>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSqlQuery>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QtSql>

#include "Windows.h"
#include "Psapi.h"
#include "WinUser.h"


class MyServer : public QObject {
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    QJsonDocument doc;
    QJsonParseError docError;
    QByteArray Data;
    QSqlDatabase db;

    static BOOL CALLBACK StaticEnumWindowsProc(HWND hwnd, LPARAM lParam);
    void OutputRunningPrograms();
    void RunProgramToUser();
    void RunProgramFromUser(QString);

signals:

public slots:
    void newConnection();
    void sockReady();
    void sockDisc();

private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MYTCPSERVER_H

