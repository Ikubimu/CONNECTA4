#ifndef RANKINGTABLEMODEL_H
#define RANKINGTABLEMODEL_H

#include <QAbstractTableModel>
#include "lib/connect4.h"
#include "lib/connect4dao.h"
#include <QLineEdit>
#include "language.h"

class rankingTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit rankingTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    void addUser(const Player* user);
    void set_lineEdit(QLineEdit* searchLine);
    Player* getPlayer(int row);
    QList<Round*> getPlayerRounds(QString NickName);

    void update_users();

private:
    Connect4DAO* db = {nullptr}; //pointer to database
    QLineEdit* searchLine;
    QList<Player*> users;

    enum Colum
    {
        IMG=0,
        NAME,
        POINTS,
        COLCOUNT
    };



};

#endif // RANKINGTABLEMODEL_H
