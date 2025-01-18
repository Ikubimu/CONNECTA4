#include "rankinguserdata.h"
#include "ui_rankinguserdata.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>

rankingUserData::rankingUserData(QString NickName, QList<Round*> partidas, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rankingUserData)
    , NickName(NickName)
{
    ui->setupUi(this);
    NickNameLabel.setText(NickName);

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *layoutV = new QVBoxLayout(this);

    layout->addStretch(1);
    layout->addWidget(&NickNameLabel,1);
    this->setLayout(layout);
}

rankingUserData::~rankingUserData()
{
    delete ui;
}

void rankingUserData::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(Qt::gray);
    QRect geom = geometry();

    int size = geom.height()*0.5;
    int x0 = geom.width()/4 - size/2;
    int y0 = geom.height()/2 - size/2;
    painter.drawEllipse(x0,y0,size,size);
}


