#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tree.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(_socket, &QTcpSocket::disconnected, _socket, &QTcpSocket::deleteLater);

    GetConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetConnection()
{
    const QString IP_ADDRESS = "192.168.0.101";
    const int PORT = 2323;

    _socket->connectToHost(IP_ADDRESS, PORT);
}

void MainWindow::on_Connect_clicked()
{
    SendToServer("Data is read");
}

void MainWindow::SendToServer(QString str)
{
    _data.clear();
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);
    out << str;

    _socket->write(_data);
}

void MainWindow::slotReadyRead()
{
    const int treeLevelsCount = 3;

    _model = new ObjectTreeModel(this);

    QStringList cols;
    cols << "objectName";
    _model->setColumns(cols);

    QObject* net = new QObject();
    net->setObjectName("net");
    _model->addItem(net, QModelIndex());
    ui->treeView->setModel(_model);

    AddDataToTree(net, treeLevelsCount);
}

ObjectTreeModel::ObjectTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    _rootItem = new QObject(this);
}

void ObjectTreeModel::setColumns(QStringList cols)
{
    _columns = cols;
}

void ObjectTreeModel::addItem(QObject *item, const QModelIndex &parentIdx)
{
     beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));
     item->setParent(objByIndex(parentIdx));
     endInsertRows();
}

QObject *ObjectTreeModel::objByIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return _rootItem;

    return static_cast<QObject*>(index.internalPointer());
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QObject* parentObj = objByIndex(parent);
    return createIndex(row, column, parentObj->children().at(row));
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &child) const
{
    QObject* childObj = objByIndex(child);
    QObject* parentObj = childObj->parent();

    if (parentObj == _rootItem)
        return QModelIndex();

    QObject* grandParentObj = parentObj->parent();
    int row = grandParentObj->children().indexOf(parentObj);
    return createIndex(row, 0, parentObj);
}

int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
     return objByIndex(parent)->children().count();
}

int ObjectTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _columns.count();
}

QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return objByIndex(index)->property(_columns.at(index.column()).toUtf8());

    return QVariant();
}

void MainWindow::AddDataToTree(QObject* node, int count)
{
    if (count == 0)
        return;

    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_5_1);

    if (in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;

        QObject* item = new QObject(node);
        item->setProperty("objectName", str);
        --count;

        if (count == 1)
        {
            QObject* item_1 = new QObject(node);
            item_1->setProperty("objectName", str + " (2)");
            QObject* item_2 = new QObject(node);
            item_2->setProperty("objectName", str + " (3)");
        }

        if (count == 0)
        {
            QObject* item_1 = new QObject(node);
            item_1->setProperty("objectName", str + " (2)");
        }

        AddDataToTree(item, count);
    }
    else
    {
        ui->lineEdit->setText("Reading error!");
    }
}

void MainWindow::on_treeView_collapsed(QModelIndex const&)
{

}
