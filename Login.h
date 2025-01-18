#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "Register.h"
#include "lib/connect4.h"

class LoginPage : public QDialog
{
    Q_OBJECT

public:
    LoginPage(QWidget *parent = nullptr,Player* players_playing[2] = nullptr);

signals:
    void Login_succesful(Player* player);
    void requestRegisterPage();
private:
    //method to check the login
    void handleLogin();
    //method to change from the windows to the Register_Window
    void change_to_register();
    QGridLayout *layout;
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *RegisterButton;
    QPushButton *ForgetPasswordButton;
    QLabel *usernameErrorLabel = nullptr;
    QLabel *passwordErrorLabel = nullptr;
    Player      *Players[2];
};

#endif // LOGIN_H
