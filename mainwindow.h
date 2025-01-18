#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "gameboard.h"
#include "rankingtable.h"
#include "userswidget.h"
#include "Login.h"

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

private:
    Ui::MainWindow *ui;
    GameBoard board;
    rankingTable rank;
    usersWidget userL, userR;
    LoginPage *loginPage;
    QPushButton *openLoginButton;
    void openLoginPage();
    void openRegisterPage();
    void handleLoginSuccess(Player* Player);
    void handleRegisterSuccess(Player* Player);
    Player* players_playing[2]; // will be initialized to nullptr at the beginning
};
#endif // MAINWINDOW_H
