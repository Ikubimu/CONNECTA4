#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "gameboard.h"
#include "rankingtable.h"
#include "userswidget.h"
#include "Login.h"
#include "Forgot_Password.h"
#include "gamehistory.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "language.h"
#include "settinguser.h"
#include "lib/connect4.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override; // Declaración del resizeEvent
    void updateSettingsWidgetPosition();

private:
    Ui::MainWindow *ui;
    GameBoard board;
    rankingTable rank;
    usersWidget *users;
    QPushButton *historyButton, *colorPickerButton;
    Player* players_playing[2]; // will be initialized to nullptr at the beginning
    SettingsUser settingsWidget;
    gameHistory *hist{nullptr};


    void onChangePieceColor();

    QWidget* buildMainWidget();
    QPushButton *settingsButton;
    void catchSignal();

private slots:
    void receive_result(GameBoard::results data);
    void change_language_signal(int idioma);
};

#endif // MAINWINDOW_H
