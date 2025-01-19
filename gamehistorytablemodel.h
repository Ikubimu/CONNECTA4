#ifndef GAMEHISTORYTABLEMODEL_H
#define GAMEHISTORYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "lib/connect4dao.h"

class gameHistoryTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit gameHistoryTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    enum searchFilter
    {
        ALL=0,
        WON,
        LOST
    };

    void updateData(QString name, QDate firstDate, QDate secondDate, searchFilter filter);



private:
    Connect4DAO *db{nullptr};
    QList<Round*> rounds;

    enum Colum
    {
        DATE=0,
        WINNER,
        LOSER,
        COLCOUNT
    };
};

#endif // GAMEHISTORYTABLEMODEL_H
