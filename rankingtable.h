#ifndef RANKINGTABLE_H
#define RANKINGTABLE_H
#include <QWidget>
#include <vector>
#include <QTableView>
#include <QMouseEvent>
#include <QLineEdit>
#include "rankingtablemodel.h"

class rankingTable : public  QTableView
{
    Q_OBJECT
public:
    rankingTable(QWidget *parent = nullptr);
    void addUser(const Player* user);

    private:
        Connect4 *db{nullptr};
        rankingTableModel* model{nullptr};
        QLineEdit* searchLine{nullptr};


        void onCellClicked(const QModelIndex &index);

};

#endif // RANKINGTABLE_H
