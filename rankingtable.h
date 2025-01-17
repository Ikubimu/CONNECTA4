#ifndef RANKINGTABLE_H
#define RANKINGTABLE_H
#include <QWidget>
#include <vector>
#include <QTableView>
#include "rankingtablemodel.h"

class rankingTable : public  QTableView
{
    Q_OBJECT
public:
    rankingTable(QWidget *parent = nullptr);
    void addUser(const QString name);

    private:
        Connect4 *db{nullptr};
        rankingTableModel* model{nullptr};
};

#endif // RANKINGTABLE_H
