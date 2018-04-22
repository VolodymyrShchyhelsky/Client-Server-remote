#include "mainwindow.h"

// Clicked (on element of list) event listener
void listControl::onListClicked (const QModelIndex &i) {
    QListWidget *w = dynamic_cast <QListWidget*> (sender());
    QListWidgetItem *item = w->takeItem(i.row());
    qDebug() << item->text();
    QByteArray RunThis = "{\"program\":\"runThis\",\"runThis\":\"" + item->text().toUtf8() + "\"}";
    mainwin->socket->write(RunThis);
    mainwin->socket->waitForBytesWritten(500);
}
