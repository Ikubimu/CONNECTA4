#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include "language.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QImage>
#include <QStackedWidget>
#include <QPainter>
#include "Register.h"
#include "Login.h"
#include "Forgot_Password.h"
#include "lib/connect4.h"
#include "edit_profile.h"

enum start_player{
    first_player,
    second_player,
    random
};
enum actual_situation{
    No_players,
    One_player,
    Two_players,
};
class usersWidget : public  QWidget
{
    Q_OBJECT
public:
    usersWidget(QWidget *parent = nullptr);
    start_player get_who_starts();
    int get_players(Player* players[2]);
private:

    actual_situation actual;
    Player  *players_playing[2];
    Player *robot;

    QComboBox *whoStarts;

    QImage AvatarRight;
    QImage AvatarLeft;

    QLabel *rightAvatarLabel;
    QLabel *leftAvatarLabel;
    QPushButton *rightButtonLogin;
    QPushButton *rightEditprofile;
    QPushButton *rightlog_out;
    QPushButton *leftButtonLogin;
    QPushButton *leftEditprofile;
    QPushButton *leftlog_out;

    QVBoxLayout *mainLayout;

    QWidget *middleContainer;
    void openLoginPage();
    void openRegisterPage();
    void handleLoginSuccess(Player *player);
    void openForgotPasswordPage();
    void openConfigureProfile();
    void log_out();
    void updateWidget(actual_situation x);
    void setupWidgets();
    void setupNoPlayersWidget();
    void setupOnePlayerWidget();
    void setupTwoPlayersWidget();
    void openEditProfilePage(Player* player);
    void handleEditProfilePage();
    QPixmap createCircularPixmap(const QImage &image); // hecho con IA

};

#endif // USERSWIDGET_H
