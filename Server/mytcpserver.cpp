#include "mytcpserver.h"

MyServer::MyServer(QObject *parent):
    QObject(parent)
{
    // Data base of pathes to programs
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("E:/QT/Server/PathDataBase.db");
    db.open();

    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any,5555)) {
        qDebug()<<"NotListen";
    } else {
        qDebug()<<"Listen";
    }
}

void MyServer::newConnection()
{
    qDebug()<<"User conected";
    socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this , SLOT(sockReady()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
}

//Receive command from client in JSON format
void MyServer::sockReady()
{
    Data = socket->readAll();
    doc = QJsonDocument::fromJson(Data,&docError);

    if (docError.errorString().toInt() == QJsonParseError::NoError) {
        if (doc.object().value("program")=="openNow") {
            OutputRunningPrograms();
        }
        else if(doc.object().value("program")=="runNow") {
            RunProgramToUser();
        }
        else if(doc.object().value("program")=="runThis") {
            RunProgramFromUser(doc.object().value("runThis").toString());
        }
    } else {
        qDebug() << "Json Error \n";
    }
}

//Pass all names of program which user can run (from database)
void MyServer::RunProgramToUser() {
    if(db.isOpen())
    {
        QSqlQuery query;
        if(query.exec("SELECT Name FROM folder_path"))
        {
            QString nameBase;
            while(query.next()) {
                QString name = "{\"name\":\"" + query.value(0).toString() + "\"},";
                nameBase = nameBase+name;
            }
            QByteArray RPTU="{\"connect\":\"ToRun\",\"programYouCanRun\":[" + nameBase.toUtf8() + "{\"name\":\"erorname\"}]}";
            socket->write(RPTU);
        } else {
            qDebug() << "SELECT trouble";
        }
    } else {
        qDebug() << "DB isnt open";
    }
}

//Recive name of program which server must run -> find path in data base -> run using "system"
void MyServer::RunProgramFromUser(QString name) {
    if(db.isOpen()) {
        QSqlQuery query;
        QString sqlString = "SELECT Path FROM folder_path WHERE name = '" + name + "'";
        if(query.exec(sqlString)) {
            query.first();
            QString path = query.value(0).toString();
            const QString s = "start " + path;
            system(s.toStdString().c_str());
        } else {
            qDebug() << "Select trouble";
        }
    } else {
        qDebug() << "Data Base isnt open";
    }
}


//Get running processes using "EnumWindows" function -> convert from file to json format -> send to client
void MyServer::OutputRunningPrograms() {
    QFile file;
    file.setFileName("E:\\Program.json");
    file.open(QIODevice::Append);
    file.resize(0);
    file.close();

    EnumWindows(StaticEnumWindowsProc, 0);

    file.setFileName("E:\\Program.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fromFile = file.readAll();
        QByteArray RunPrograms = "{\"connect\":\"ToOutput\",\"result\":[" + fromFile  + "{\"name\":\"erorname\"}]}";
        socket->write(RunPrograms);
        socket->waitForBytesWritten(5000);
    }
    file.close();
}

//Get running processes and write them to file
BOOL MyServer::StaticEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    WCHAR title[255];
    GetWindowText(hwnd, title, 255);
    QString currentHWMD = "{\"name\":\"";

    if(QString::fromWCharArray(title) != "")
    {
        currentHWMD += QString::fromWCharArray(title);
        currentHWMD += "\"},";

        QFile file;
        file.setFileName("E:\\Program.json");
        if (file.open(QIODevice::Append)) {
            QTextStream outStream(&file);
            outStream << currentHWMD;
        }
        file.close();

    }
    return TRUE;
}

void MyServer::sockDisc()
{
    qDebug() << "Disconect";
    socket->deleteLater();
}

