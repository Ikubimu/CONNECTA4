#ifndef GAMEHISTORY_H
#define GAMEHISTORY_H

#include <QWidget>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableView>
#include <QPushButton>
#include "gamehistorytablemodel.h"

namespace Ui {
class gameHistory;
}

class gameHistory : public QWidget
{
    Q_OBJECT

public:
    explicit gameHistory(QWidget *parent = nullptr);
    ~gameHistory();

private:
    Ui::gameHistory *ui;

    QLineEdit getNickName;
    QDateEdit FirstDate, SecondDate;
    QCheckBox CheckWin, CheckLost;
    QPushButton SearchButton;
    QTableView HistoryTable;
    gameHistoryTableModel model;


    void SearchButtonCallback();
    void textEditedCallback();
};

#endif // GAMEHISTORY_H
