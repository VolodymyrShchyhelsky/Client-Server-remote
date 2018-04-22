#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetListControl();
    socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisconect()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("localhost",5555); //If server is not localhost, change "localhost" on IP of server
}

void MainWindow::sockDisconect() {
    socket->deleteLater();
}

//Receive command from server in JSON format
void MainWindow::sockReady() {
    if(socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        doc = QJsonDocument::fromJson(Data, &docError);
        if (docError.errorString().toInt() == QJsonParseError::NoError)
        {
            if(doc.object().value("connect").toString()=="ToOutput")
                OutputRunningPrograms();
            if(doc.object().value("connect").toString()=="ToRun")
                RunProgram();
        } else
        {
            qDebug() << "Whoops something wrong \n Json Error : ";
            qDebug() << docError.errorString().toInt();
        }
    }
}

//Request server to show running processes
void MainWindow::on_pushButton_2_clicked()
{
    if (socket->isOpen()) {
        socket->write("{\"program\":\"openNow\"}");
        socket->waitForBytesWritten(500);
    } else {
        qDebug() << "Conect to server pls";
    }
}

//Request server to show list of program which you can run
void MainWindow::on_pushButton_3_clicked()
{
    if (socket->isOpen()) {
        socket->write("{\"program\":\"runNow\"}");
        socket->waitForBytesWritten(500);
    } else {
        qDebug() << "Conect to server pls";
    }
}

//Show running processes on server
void MainWindow::OutputRunningPrograms() {
    QStandardItemModel* model = new QStandardItemModel(nullptr);
    model->setHorizontalHeaderLabels(QStringList()<<"name");

    QJsonArray docArr = doc.object().value("result").toArray();
    for(int i=0; i<docArr.count()-1; ++i)
    {
        bool flag=false; //Discard processes which have the same name or processes which consist of "?" symbols
        if(docArr[i].toObject().value("name").toString()[1]==QChar('?'))
            flag=true;
        else
        for(int j=i+1; j<docArr.count(); ++j)
            if(docArr[i].toObject().value("name").toString() == docArr[j].toObject().value("name").toString())
                flag=true;

        if(!flag)
        {
            QStandardItem* newitem = new QStandardItem(docArr[i].toObject().value("name").toString());
            model->appendRow(newitem);
        }
    }

    ui->tableView->setModel(model);
}

//Fill the list of program which you can run
void MainWindow::RunProgram() {
    ui->listWidget->clear();
    QJsonArray docArr = doc.object().value("programYouCanRun").toArray();
    for(int i=0; i<docArr.count()-1; ++i)
        ui->listWidget->addItem(docArr[i].toObject().value("name").toString());
}

//Set listwidget control
void MainWindow::SetListControl() {
    listcontrol = new listControl();
    listcontrol->listwidget = ui->listWidget;
    listcontrol->mainwin = this;
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)),
            listcontrol, SLOT(onListClicked(QModelIndex)));
}


