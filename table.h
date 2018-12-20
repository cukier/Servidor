#ifndef TABLE_H
#define TABLE_H

#include <QString>

enum Roles {
    docentryRole = Qt::UserRole + 1,
    StcCodeRole,
    porcentRole,
    ItemCodeRole,
    StateRole,
    ItemNameRole
};

struct TableRow
{
    QString docentry;
    QString StcCode;
    QString porcent;
    QString ItemCode;
    QString State;
    QString ItemName;

    bool operator==(const TableRow &other) const
    {
        return ItemCode == other.ItemCode;
    }
};

inline QDataStream &operator<<(QDataStream &stream, const TableRow &tablerow)
{
    return stream << tablerow.docentry
                  << tablerow.StcCode
                  << tablerow.porcent
                  << tablerow.ItemCode
                  << tablerow.State
                  << tablerow.ItemName;
}

inline QDataStream &operator>>(QDataStream &stream, TableRow &tablerow)
{
    return stream >> tablerow.docentry
                  >> tablerow.StcCode
                  >> tablerow.porcent
                  >> tablerow.ItemCode
                  >> tablerow.State
                  >> tablerow.ItemName;
}
#endif // TABLE_H
