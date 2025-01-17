#include "rankingtablemodel.h"

rankingTableModel::rankingTableModel(QObject *parent)
{
    db = &(Connect4::getInstance());
    addUser(db->getPlayer("Player1"));
}

int rankingTableModel::rowCount(const QModelIndex &)const
{
    return static_cast<int>(users.size());
}


int rankingTableModel::columnCount(const QModelIndex &)const
{
    return COLCOUNT;
}


QVariant rankingTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= static_cast<int>(users.size()))
        return QVariant();

    Player* player = users.at(index.row());

    if (role == Qt::DisplayRole) {
        qDebug() << "Jugador " << player->getNickName()<<" "<<player->getPoints();
        switch (index.column()) {
        case RANK:
            return QString::number(1);
        case NAME:
            return player->getNickName();
        case POINTS:
            return QString::number(player->getPoints());
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant rankingTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case RANK:
            return QString("Pos");
        case NAME:
            return QString("Name");
        case POINTS:
            return QString("Points");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags rankingTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;


    return flags;
}

void rankingTableModel::addUser(Player* user)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    users.push_back(user);
    endInsertRows();
}




