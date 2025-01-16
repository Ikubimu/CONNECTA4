#include "rankingtable.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

#define SIZE_W 0.9
#define SIZE_H 0.6

rankingTable::rankingTable(QWidget *parent)
{

}

void rankingTable::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(Qt::gray);

    QRect geom = geometry();


    int size_h = geom.height()*SIZE_H;
    int size_w = geom.width()*SIZE_W;
    int x0 = geom.width()/2 - size_w/2;
    int y0 = geom.height()/2 - size_h/2;
    painter.drawRect(x0,y0,size_w,size_h);

}
