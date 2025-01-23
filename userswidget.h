#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include "language.h"

class usersWidget : public  QWidget
{
    Q_OBJECT
public:
    usersWidget(QWidget *parent = nullptr);
    protected:
        void paintEvent(QPaintEvent *event) override;
};

#endif // USERSWIDGET_H
