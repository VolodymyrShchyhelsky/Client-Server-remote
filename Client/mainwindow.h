#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonParseError>
#include <QStandardItem>
#include <QJsonDocument>
#include <QMainWindow>
#include <QMessageBox>
#include <QJsonObject>
#include <QListWidget>
#include <QStringList>
#include <QMetaObject>
#include <QTcpSocket>
#include <QTableView>
#include <QJsonArray>
#include <QWidget>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QChar>

#include "listcontrol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    listControl *listcontrol; //List of program which you can run
    QTcpSocket* socket;
    QByteArray Data;
    QJsonDocument doc;
    QJsonParseError docError;

    void OutputRunningPrograms();
    void RunProgram();
    void SetListControl();

public slots:
    void sockReady();
    void sockDisconect();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
