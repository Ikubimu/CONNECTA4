#include "rankingtable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QVBoxLayout>
#include <QTimer>

#define SIZE_W 0.7
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

    QTimer *timer = new QTimer(this);
    timer->setInterval(100);

    // Funcíon periódica que evalua la caja de búsqueda
    connect(timer, &QTimer::timeout, this, &rankingTable::updateTable);


    timer->start();

}


void rankingTable::resizeEvent(QResizeEvent *event)
{
    QRect geom = geometry();
    this->setFixedWidth(geom.height()*SIZE_W);
}

void rankingTable::updateTable()
{
    model.update_users();
    searchLine.setPlaceholderText(Labels::search);
}


void rankingTable::addUser(const Player* user)
{
    model.addUser(user);
}



