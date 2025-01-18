#include "rankingtablemodel.h"
#include <Qtimer>
#include <QPixmap>




rankingTableModel::rankingTableModel(QObject *parent)
{
    db = &(Connect4DAO::getInstance());
    Connect4& DB_Instance = Connect4::getInstance();
    DB_Instance.registerPlayer("Player2", "player1@example.com", "Password123!", QDate(1990, 1, 1), 200);
    DB_Instance.registerPlayer("Player3", "player1@example.com", "Password123!", QDate(1990, 1, 1), 100);

    addUser(DB_Instance.getPlayer("Player1"));
    addUser(DB_Instance.getPlayer("Player2"));
    addUser(DB_Instance.getPlayer("Player3"));
    users=db->getRanking();
    for(int i=0; i<users.size(); i++)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        endInsertRows();
    }

    QTimer *timer = new QTimer(this);
    timer->setInterval(100);

    // Funcíon periódica que evalua la caja de búsqueda
    connect(timer, &QTimer::timeout, this, &rankingTableModel::update_users);


    timer->start();

    // Iniciar el temporizador
}

void rankingTableModel::update_users()
{

    users=db->getRanking();
    QString text = searchLine->text();
    if(text != prev_text)
    {
        prev_text = text;
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



    if (role == Qt::DisplayRole) {

        Player* player = users.at(index.row());
        switch (index.column()) {
        case IMG:
            return QString::number(index.row() + 1);
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
        case IMG:
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






