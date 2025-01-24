#include "gamehistorytablemodel.h"

gameHistoryTableModel::gameHistoryTableModel(QObject *parent)
 : QAbstractTableModel(parent)
{
    db = &Connect4DAO::getInstance();
}

static QList<Round*> remove_repeated(QList<Round*> raw_rounds)
{
    QList<Round*> rounds = raw_rounds;
    for(int i=0; i<rounds.size(); i++)
    {
        Round* round = rounds.at(i);
        for(int j=i+1; j<rounds.size(); j++)
        {
            Round* r = rounds.at(j);
            if((round->getTimestamp() == r->getTimestamp()) && (round->getWinner()->getNickName() == r->getWinner()->getNickName()) && (round->getLoser()->getNickName() == r->getLoser()->getNickName()))
            {
                rounds.removeAt(j);
                j--;

            }
        }
    }

    return rounds;
}

static QList<Round*> filter_date(QList<Round*> raw_rounds, QDate firstDate, QDate secondDate)
{
    QList<Round*> rounds;
    for(int i=0; i<raw_rounds.size(); i++)
    {
        Round* round = raw_rounds.at(i);
        if((round->getTimestamp().date() >= firstDate) && (round->getTimestamp().date() <= secondDate))
        {
            rounds.push_back(round);
        }
    }
    return rounds;
}

static QList<Round*> filter_state(QList<Round*> raw_rounds, QString name, gameHistoryTableModel::searchFilter state)
{

    QList<Round*> rounds;
    for(int i=0; i<raw_rounds.size(); i++)
    {
        Round* round = raw_rounds.at(i);
        if((state == gameHistoryTableModel::LOST) && (round->getLoser()->getNickName()==name))
        {
            rounds.push_back(round);
        }
        else if((state == gameHistoryTableModel::WON) && (round->getWinner()->getNickName()==name))
        {
            rounds.push_back(round);
        }
    }

    return rounds;
}
static QList<Round*> order_by_date(QList<Round*> raw_rounds)
{

}

void gameHistoryTableModel::updateData(QString name, QDate firstDate, QDate secondDate, searchFilter filter)
{
    beginResetModel();
    beginRemoveRows(QModelIndex(), 0, rowCount());
    endRemoveRows();

    QList<Player*> users = db->getRanking();
    QList<Round*> raw_rounds;
    if(name.isEmpty())
    {
        for(int i=0; i<users.size(); i++)
        {
            raw_rounds.append(db->getRoundsForPlayer(users.at(i)->getNickName()));
        }
    }
    else
    {
        raw_rounds = db->getRoundsForPlayer(name);
    }
    raw_rounds = remove_repeated(raw_rounds);
    raw_rounds = filter_date(raw_rounds, firstDate, secondDate);
    if(filter)
    {
        raw_rounds = filter_state(raw_rounds, name, filter);
    }
    beginInsertRows(QModelIndex(), 0, raw_rounds.size());
    endInsertRows();
    rounds = raw_rounds;
    endResetModel();

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
            return QString(Labels::date);
        case WINNER:
            return QString(Labels::winner);
        case LOSER:
            return QString(Labels::loser);
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
