#ifndef CLISTMODEL_H
#define CLISTMODEL_H

#include <QAbstractListModel>

#include "table.h"

class CListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CListModel(QObject *parent = nullptr);
    ~CListModel();

    void addData(const TableRow& table);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<TableRow> m_tables;
};

#endif // CLISTMODEL_H
