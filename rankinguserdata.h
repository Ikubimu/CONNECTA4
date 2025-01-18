#ifndef RANKINGUSERDATA_H
#define RANKINGUSERDATA_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include "lib/connect4dao.h"

namespace Ui {
class rankingUserData;
}

class rankingUserData : public QWidget
{
    Q_OBJECT

public:
    explicit rankingUserData(QString NickName, QList<Round*> partidas, QWidget *parent = nullptr);
    ~rankingUserData();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::rankingUserData *ui;
    QString NickName;
    QLabel NickNameLabel;

};

#endif // RANKINGUSERDATA_H
