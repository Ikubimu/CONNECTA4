#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QImage>
#include <QPainter>
#include "Register.h"
#include "Login.h"
#include "Forgot_Password.h"
#include "lib/connect4.h"
enum start_player{
    first_player,
    second_player,
    random
};
class usersWidget : public  QWidget
{
    Q_OBJECT
public:
    usersWidget(QWidget *parent = nullptr);

private:
    enum actual_situation{
        No_players,
        One_player,
        Two_players,
    };
    actual_situation actual;
    Player  *players_playing[2];
    Player *robot;
    QComboBox *whoStarts;
    QComboBox *Difficulty_Level_Robot;
    QImage AvatarRight;
    QImage AvatarLeft;
    QPushButton *rightButtonLogin;
    QPushButton *rightEditprofile;
    QPushButton *rightlog_out;
    QPushButton *leftButtonLogin;
    QPushButton *leftEditprofile;
    QPushButton *leftlog_out;
    QHBoxLayout *mainWidget;
    QVBoxLayout *left_widget;
    QVBoxLayout *right_widget;
    start_player get_who_starts();
    void openLoginPage();
    void openRegisterPage();
    void handleLoginSuccess(Player *player);
    void openForgotPasswordPage();
    void openConfigureProfile();
    void log_out();
    void updateWidget(actual_situation x);
    QPixmap createCircularPixmap(const QImage &image); // hecho con IA

};

#endif // USERSWIDGET_H
