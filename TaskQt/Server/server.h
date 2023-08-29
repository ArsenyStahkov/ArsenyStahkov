#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QSqlDatabase>
#include <QFile>
#include <QCoreApplication>
#include <QtXml>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

class Server : public QTcpServer
{
    Q_OBJECT;
    QVector <QTcpSocket*> _sockets;
    QByteArray _data;
    QSqlDatabase _db;
    QSqlQuery* _sqlQuery;
    void SendToClient();
    //void GetXMLFiles();
    void ConvertXMLToSQL();
    void FillDataBaseByXML(QDomNode node, int count);
    void CreateDataBase();

public:
    QTcpSocket* socket;
    Server();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif
