#include "rankingtablemodel.h"
#include <Qtimer>
#include <QPixmap>
#include <QImage>




rankingTableModel::rankingTableModel(QObject *parent)
{
    db = &(Connect4DAO::getInstance());
    Connect4& DB_Instance = Connect4::getInstance();
    Player* p1 = DB_Instance.getPlayer(Labels::player_1);
    users=db->getRanking();
    for(int i=0; i<users.size(); i++)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        endInsertRows();
    }

}

void rankingTableModel::update_users()
{

    users=db->getRanking();
    for(int i=0;i<users.size(); i++)
    {
        if(users.at(i)->getNickName() == "CPU")
        {
            users.removeAt(i);
            break;
        }
    }

    QString text = searchLine->text();

    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    endRemoveRows();
    for(int i=0; i<users.size(); i++)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        endInsertRows();
    }
    for(int i=0; i<users.size(); i++)
    {
        QString name = users.at(i)->getNickName();
        if(!name.contains(text))
        {
            beginRemoveRows(QModelIndex(), i, i);
            users.removeAt(i);
            endRemoveRows();
            i--;
        }
    }






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


        switch (index.column()) {
        case NAME:
            return player->getNickName();
        case POINTS:
            return QString::number(player->getPoints());
        default:
            return QVariant();
        }
    }
    if(role==Qt::DecorationRole)
    {
        if(index.column() == IMG)
        {
            QImage foto = player->getAvatar();
            QPixmap pixmap = QPixmap::fromImage(foto);
            return pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
        case IMG:
            return QString("");
        case NAME:
            return QString("ID");
        case POINTS:
            return QString(Labels::points);
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

void rankingTableModel::addUser(const Player* user)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    db->addPlayer(*user);
    endInsertRows();
}

void rankingTableModel::set_lineEdit(QLineEdit* searchLine)
{
    this->searchLine = searchLine;
}


Player* rankingTableModel::getPlayer(int row)
{
    return users.at(row);
}

QList<Round*> rankingTableModel::getPlayerRounds(QString NickName)
{
    return db->getRoundsForPlayer(NickName);
}






