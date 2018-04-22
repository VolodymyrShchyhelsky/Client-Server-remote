#ifndef LISTCONTROL_H
#define LISTCONTROL_H

#include <QListWidget>
#include <QModelIndex>
#include <QVariant>
#include <QObject>
#include <QString>
#include <QDebug>

class MainWindow;

//List of program which you can run
class listControl : public QObject
{
    Q_OBJECT
public:
    listControl(QObject* parent = nullptr) : QObject(parent) {}
    QListWidget *listwidget;
    MainWindow *mainwin;
public slots:
    void onListClicked (const QModelIndex &i);
};

#endif // LISTCONTROL_H
