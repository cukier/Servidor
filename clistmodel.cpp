#include "clistmodel.h"

CListModel::CListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

CListModel::~CListModel()
{

}

void CListModel::addData(const TableRow &table)
{
    for (auto i : m_tables) {
        if (i.State == table.State&&
                i.StcCode == table.StcCode &&
                i.porcent == table.porcent &&
                i.ItemCode == table.ItemCode &&
                i.ItemName == table.ItemName &&
                i.docentry == table.docentry) {
            return;
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_tables.append(table);
    endInsertRows();
}

int CListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tables.count();
}

QVariant CListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_tables.count())
        return QVariant();

    if (role == docentryRole) {
        const QString& docentry = m_tables.value(index.row()).docentry;
        return docentry;
    } else if (role == StcCodeRole) {
        const QString& StcCode = m_tables.value(index.row()).StcCode;
        return StcCode;
    } else if (role == porcentRole) {
        const QString& porcent = m_tables.value(index.row()).porcent;
        return porcent;
    } else if (role == ItemCodeRole) {
        const QString& ItemCode = m_tables.value(index.row()).ItemCode;
        return ItemCode;
    } else if (role == StateRole) {
        const QString& State = m_tables.value(index.row()).State;
        return State;
    } else if (role == ItemNameRole) {
        const QString& ItemName = m_tables.value(index.row()).ItemName;
        return ItemName;
    }

    return QVariant();
}

QHash<int, QByteArray> CListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[docentryRole] = "docentry";
    roles[StcCodeRole] = "StcCode";
    roles[porcentRole] = "porcent";
    roles[ItemCodeRole] = "ItemCode";
    roles[StateRole] = "StateCode";
    roles[ItemNameRole] = "ItemName";
    return roles;
}
