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
    FirstDate.setMaximumDate(QDate::currentDate());

    SecondDate.setCalendarPopup(true);
    SecondDate.setDate(QDate::currentDate());
    SecondDate.setDateRange(QDate::currentDate(), QDate::currentDate());

    connect(&FirstDate, &QDateEdit::dateChanged, this, &gameHistory::dateEditedCallback);
    connect(&SecondDate, &QDateEdit::dateChanged, this, &gameHistory::dateEditedCallback);

    HistoryTable.setModel(&model);

    getNickName.setPlaceholderText(QString("NickName (Optional)"));
    connect(&getNickName, &QLineEdit::editingFinished, this, &gameHistory::textEditedCallback);

    CheckWin.setText("Won");
    CheckWin.setEnabled(false);
    CheckLost.setText("Lost");
    CheckLost.setEnabled(false);
    CheckAll.setText("All");
    CheckAll.setChecked(true);
    CheckAll.setEnabled(false);

    checks.addButton(&CheckAll);
    checks.addButton(&CheckWin);
    checks.addButton(&CheckLost);
    checks.setExclusive(true);

    SearchButton.setText(QString("Search"));
    connect(&SearchButton, &QPushButton::clicked, this, &gameHistory::SearchButtonCallback);

    layoutHCheck->addWidget(&CheckWin);
    layoutHCheck->addWidget(&CheckLost);
    layoutHCheck->addWidget(&CheckAll);

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
        CheckAll.setChecked(true);
        CheckAll.setEnabled(false);

        CheckWin.setChecked(false);
        CheckWin.setEnabled(false);

        CheckLost.setChecked(false);
        CheckLost.setEnabled(false);
    }
    else
    {
        CheckAll.setEnabled(true);
        CheckWin.setEnabled(true);
        CheckLost.setEnabled(true);
    }
}

void gameHistory::dateEditedCallback()
{
    FirstDate.setMaximumDate(SecondDate.date());
    SecondDate.setDateRange(FirstDate.date(), QDate::currentDate());
}
