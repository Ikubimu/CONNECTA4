#include "rankingtable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QVBoxLayout>

#define SIZE_W 0.9
#define SIZE_H 0.6

rankingTable::rankingTable(QWidget *parent)
    : QTableView(parent)
{
    model = new rankingTableModel();
    this->setModel(model);
    searchLine = new QLineEdit(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addSpacing(500);
    layout->addWidget(searchLine);

    this->setLayout(layout);

    model->set_lineEdit(searchLine);
    this->resizeColumnsToContents();
    QRect geom = geometry();
    this->setFixedHeight(geom.height()*SIZE_H);
}


void rankingTable::addUser(const Player* user)
{
    model->addUser(user);
}



