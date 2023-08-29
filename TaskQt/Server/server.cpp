#include "server.h"

Server::Server()
{
    const int PORT = 2323;

    if (this->listen(QHostAddress::Any, PORT))
        qDebug() << "Server turns on";
    else
        qDebug() << "Error!";

    CreateDataBase();
    ConvertXMLToSQL();
}

void Server::ConvertXMLToSQL()
{
    const QString path = "../";

    QFile file(path + "1.1.1.1.xml");

    if (file.open(QIODevice::ReadOnly))
        qDebug() << "XML file is open";
    else
        qDebug() << "XML file is not open!";
    _sqlQuery = new QSqlQuery(_db);

    QDomDocument XMLDOM;
    XMLDOM.setContent(&file);
    file.close();

    QDomElement root = XMLDOM.documentElement();

    FillDataBaseByXML(root, 3);
}

void Server::FillDataBaseByXML(QDomNode node, int count)
{
    if (count == 0)
        return;

    for (int i = 0; i < node.childNodes().count(); i++)
    {
        QDomNode item = node.childNodes().item(0);

        _sqlQuery->exec("INSERT INTO " + item.nodeName() +
                        " VALUES (" +
                        item.attributes().item(0).nodeValue() +
                        ", " + item.attributes().item(1).nodeValue() +
                        ", " + item.attributes().item(2).nodeValue() +
                        ", " + item.attributes().item(3).nodeValue() +
                        ", " + item.attributes().item(4).nodeValue() +
                        ", " + item.attributes().item(5).nodeValue() +
                        ", " + item.attributes().item(6).nodeValue() +
                        ", " + item.attributes().item(7).nodeValue() +
                        ");");

        if (item.childNodes().count() > 0)
        {
            --count;
            FillDataBaseByXML(item, count);
        }
    }
}

void Server::CreateDataBase()
{
    const QString path = "../DataBases/";
    const QString dataBaseName = "net.db";

    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(path + dataBaseName);

    if (_db.open())
        qDebug() << "Data base is open";
    else
        qDebug() << "Data base is not open!";

    _sqlQuery = new QSqlQuery(_db);
}

void Server::SendToClient()
{
    QSqlQuery sqlQuery_1("SELECT Table_name FROM hierarchy WHERE Parent_id = 0;", _db);
    QSqlQuery sqlQuery_2("SELECT Table_name FROM hierarchy WHERE Parent_id = 1;", _db);
    QSqlQuery sqlQuery_3("SELECT Table_name FROM hierarchy WHERE Parent_id = 2;", _db);

    while (sqlQuery_1.next(), sqlQuery_2.next(), sqlQuery_3.next())
    {
        _data.clear();
        QDataStream out(&_data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);

        out << sqlQuery_1.value(0).toString() << sqlQuery_2.value(0).toString() << sqlQuery_3.value(0).toString();

        for (int j = 0; j < _sockets.size(); j++)
            _sockets[j]->write(_data);
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    _sockets.push_back(socket);
    qDebug() << "Client connected";
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_1);

    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "DataStream Ok";
        QString str;
        in >> str;
        qDebug() << str;

        QSqlQuery _sqlQuery(_db);
        _sqlQuery.exec("SELECT Table_name FROM hierarchy WHERE Parent_id = 0;");

        while (_sqlQuery.next())
            SendToClient();
    }
    else
    {
        qDebug() << "DataStream error!";
    }
}
