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

#include <QPainterPath>

enum start_player {
    first_player,
    second_player,
    random
};

enum actual_situation {
    No_players,
    One_player,
    Two_players,
    current,
};

class usersWidget : public QWidget {
    Q_OBJECT
public:
    usersWidget(QWidget *parent = nullptr);
    start_player get_who_starts();
    int get_players(Player* players[2]);

    void updateWidget(actual_situation x);

    void highlightPlayer(int playerIndex); // Nueva función para resaltar al jugador activo

    void setPlayerColors(const QColor &color1);

signals:
    void emit_current_players(int num);

private:
    actual_situation actual;
    Player *players_playing[2];
    Player *robot;

    QComboBox *whoStarts;

    QImage AvatarRight;
    QImage AvatarLeft;

    QLabel *rightAvatarLabel; // Miembro de la clase
    QLabel *leftAvatarLabel;  // Miembro de la clase
    QPushButton *rightButtonLogin;
    QPushButton *rightEditprofile;
    QPushButton *rightlog_out;
    QPushButton *leftButtonLogin;
    QPushButton *leftEditprofile;
    QPushButton *leftlog_out;
    QLabel *nickname;
    QLabel *CPU;
    QLabel *versus, *start;
    QVBoxLayout *mainLayout;

    QWidget *middleContainer;

    QColor p1Color; // Color de la ficha del jugador 1
    QColor p2Color; // Color de la ficha del jugador 2

    void openLoginPage();
    void openRegisterPage();
    void handleLoginSuccess(Player *player);
    void openForgotPasswordPage();
    void openConfigureProfile();
    void log_out();
    void setupWidgets();
    void setupNoPlayersWidget();
    void setupOnePlayerWidget();
    void setupTwoPlayersWidget();
    void openEditProfilePage(Player* player);
    void handleEditProfilePage();
    void drawPlayerBorder(QLabel *label, const QColor &borderColor);

    QPixmap createCircularPixmap(const QImage &image); // Hecho con IA
};

#endif // USERSWIDGET_H
