#ifndef TREE_H
#define TREE_H

#include <QAbstractItemModel>

class ObjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ObjectTreeModel(QObject* parent = nullptr);
    void setColumns(QStringList cols);
    void addItem(QObject* item, const QModelIndex &parentIdx);

protected:
    QObject* _rootItem;
    QStringList _columns;
    QObject* objByIndex(const QModelIndex &index) const;

public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif
