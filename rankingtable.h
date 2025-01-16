#ifndef RANKINGTABLE_H
#define RANKINGTABLE_H
#include <QWidget>
#include <vector>

class rankingTable : public  QWidget
{
    Q_OBJECT
public:
    rankingTable(QWidget *parent = nullptr);


    protected:
        void paintEvent(QPaintEvent *event) override;
};

#endif // RANKINGTABLE_H
