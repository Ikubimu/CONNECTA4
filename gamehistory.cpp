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

    getNickName.setPlaceholderText(QString("NickName (Optional)"));
    connect(&getNickName, &QLineEdit::editingFinished, this, &gameHistory::textEditedCallback);

    CheckWin.setText("Won");
    CheckWin.setEnabled(false);
    CheckLost.setText("Lost");
    CheckLost.setEnabled(false);

    SearchButton.setText(QString("Search"));
    connect(&SearchButton, &QPushButton::clicked, this, &gameHistory::SearchButtonCallback);

    layoutHCheck->addWidget(&CheckWin);
    layoutHCheck->addWidget(&CheckLost);

    layoutHDate->addWidget(&FirstDate);
    layoutHDate->addWidget(&SecondDate);

    layout->addLayout(layoutHDate);
    layout->addWidget(&getNickName);
    layout->addLayout(layoutHCheck);
    layout->addWidget(&SearchButton);
    layout->addWidget(&HistoryTable);


}

gameHistory::~gameHistory()
{
    delete ui;
}

void gameHistory::SearchButtonCallback()
{
    gameHistoryTableModel::searchFilter state;
    if(CheckWin.isChecked())
    {
        state = gameHistoryTableModel::WON;
    }
    else if(CheckLost.isChecked())
    {
        state = gameHistoryTableModel::LOST;
    }
    else
    {
        state = gameHistoryTableModel::ALL;
    }
    QString name = getNickName.text();
    QDate first = FirstDate.date();
    QDate second = SecondDate.date();
    model.updateData(name, first, second, state);
}

void gameHistory::textEditedCallback()
{
    if(getNickName.text().isEmpty())
    {
        CheckWin.setEnabled(false);
        CheckLost.setEnabled(false);
    }
    else
    {
        CheckWin.setEnabled(true);
        CheckLost.setEnabled(true);
    }
}
