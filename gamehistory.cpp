#include "gamehistory.h"
#include "ui_gamehistory.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

gameHistory::gameHistory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::gameHistory)
{
    ui->setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* layoutHDate = new QHBoxLayout();
    QHBoxLayout* layoutHCheck = new QHBoxLayout();


    FirstDate.setCalendarPopup(true); //Se va a mostrar el calendario como ventana emergente
    FirstDate.setDate(QDate::currentDate());

    SecondDate.setCalendarPopup(true);
    SecondDate.setDate(QDate::currentDate());

    HistoryTable.setModel(&model);

    getNickName.setPlaceholderText(QString("NickName"));
    CheckWin.setText("Won");
    CheckLost.setText("Lost");
    SearchButton.setText(QString("Search"));

    layoutHCheck->addWidget(&CheckWin);
    layoutHCheck->addWidget(&CheckLost);

    layoutHDate->addWidget(&FirstDate);
    layoutHDate->addWidget(&SecondDate);

    layout->addWidget(&getNickName);
    layout->addLayout(layoutHDate);
    layout->addLayout(layoutHCheck);
    layout->addWidget(&SearchButton);
    layout->addWidget(&HistoryTable);


}

gameHistory::~gameHistory()
{
    delete ui;
}
