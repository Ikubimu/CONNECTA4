#include "rankinguserdata.h"
#include "ui_rankinguserdata.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>

rankingUserData::rankingUserData(Player* user, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rankingUserData)
    , user(user)
{
    ui->setupUi(this);
    QPixmap pixmap = QPixmap::fromImage(user->getAvatar());
    pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    NickNameLabel.setText(user->getNickName());
    ImageLabel.setPixmap(pixmap);

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *layoutV = new QVBoxLayout(this);


    layout->addWidget(&ImageLabel,1);
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


