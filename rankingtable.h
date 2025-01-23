#ifndef RANKINGTABLE_H
#define RANKINGTABLE_H
#include <QWidget>
#include <vector>
#include <QTableView>
#include <QMouseEvent>
#include <QLineEdit>
#include "rankingtablemodel.h"
#include "language.h"

class rankingTable : public QWidget
{
    Q_OBJECT
public:
    rankingTable(QWidget *parent = nullptr);
    void addUser(const Player* user);

    private:
        Connect4 *db{nullptr};
        QTableView rankingView;
        rankingTableModel model;
        QLineEdit searchLine;


};

#endif // RANKINGTABLE_H
