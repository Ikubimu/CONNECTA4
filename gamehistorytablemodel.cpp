#include "gamehistorytablemodel.h"

gameHistoryTableModel::gameHistoryTableModel(QObject *parent)
 : QAbstractTableModel(parent)
{

}

int gameHistoryTableModel::rowCount(const QModelIndex &)const
{
    return static_cast<int>(rounds.size());
}


int gameHistoryTableModel::columnCount(const QModelIndex &)const
{
    return COLCOUNT;
}


QVariant gameHistoryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (index.row() < 0 || index.row() >= static_cast<int>(rounds.size()))
        return QVariant();


    Round* round = rounds.at(index.row());
    if (role == Qt::DisplayRole) {


        switch (index.column()) {
        case DATE:
            return round->getTimestamp();
        case WINNER:
            return round->getWinner()->getNickName();
        case LOSER:
            return round->getLoser()->getNickName();
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant gameHistoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case DATE:
            return QString("Date");
        case WINNER:
            return QString("Winner");
        case LOSER:
            return QString("Looser");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags gameHistoryTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;


    return flags;
}
