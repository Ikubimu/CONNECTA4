#include "userswidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

#define SIZE_REL 0.9

usersWidget::usersWidget(QWidget *parent)
{

}

void usersWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(Qt::gray);

    QRect geom = geometry();


    int size = geom.height()*SIZE_REL;
    int x0 = geom.width()/2 - size/2;
    int y0 = geom.height()/2 - size/2;
    painter.drawEllipse(x0,y0,size,size);

}
