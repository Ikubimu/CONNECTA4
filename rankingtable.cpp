#include "rankingtable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

#define SIZE_W 0.9
#define SIZE_H 0.6

rankingTable::rankingTable(QWidget *parent)
    : QTableView(parent)
{
    model = new rankingTableModel();
    this->setModel(model);

    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}


void rankingTable::addUser(const QString name)
{
    int a;
}
