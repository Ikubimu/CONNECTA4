#ifndef GAMEHISTORYTABLEMODEL_H
#define GAMEHISTORYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "lib/connect4.h"

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

private:
    Connect4 *db{nullptr};
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
