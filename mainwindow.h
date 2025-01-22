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

#include "settinguser.h"

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
    usersWidget userL, userR;
    QPushButton *openLoginButton;
    void openLoginPage();
    void openRegisterPage();
    void openForgotPasswordPage();
    void handleLoginSuccess(Player* Player);
    void handleRegisterSuccess(Player* Player);
    Player* players_playing[2]; // will be initialized to nullptr at the beginning
    QWidget* centralWidget;


    QWidget* buildMainWidget();
    void catchSignal();
    SettingsUser settingsWidget;
};
#endif // MAINWINDOW_H
