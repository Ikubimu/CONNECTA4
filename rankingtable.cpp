#include "rankingtable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QVBoxLayout>

#define SIZE_W 0.9
#define SIZE_H 0.6

rankingTable::rankingTable(QWidget *parent)
    :  QWidget(parent)
{
    rankingView.setModel(&model);

    QVBoxLayout *layout = new QVBoxLayout(this);
    searchLine.setPlaceholderText(Labels::search);
    layout->addWidget(&searchLine);
    layout->addWidget(&rankingView);

    this->setLayout(layout);

    model.set_lineEdit(&searchLine);
    rankingView.resizeColumnsToContents();
    QRect geom = geometry();
    this->setFixedHeight(geom.height()*SIZE_H);
}


void rankingTable::addUser(const Player* user)
{
    model.addUser(user);
}



