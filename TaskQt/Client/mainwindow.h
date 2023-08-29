#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QFile>
#include "tree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_Connect_clicked();

    void on_treeView_collapsed(const QModelIndex &index);

private:
    Ui::MainWindow* ui;
    QTcpSocket* _socket;
    QByteArray _data;
    void GetConnection();
    void SendToServer(QString str);
    void AddDataToTree(QObject* node, int count);
    ObjectTreeModel* _model;

public slots:
    void slotReadyRead();
};

#endif
