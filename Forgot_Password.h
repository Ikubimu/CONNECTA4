#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include "Register.h"
#include "lib/connect4.h"

class ForgotPasswordPage : public QDialog
{
    Q_OBJECT

public:
    ForgotPasswordPage(QWidget *parent = nullptr,Player* players_playing[2] = nullptr);

signals:
    void Login_succesful(Player* player);
private:
    //method to check the login
    void handleForgot();
    QGridLayout *layout;
    QLineEdit *usernameField;
    QLineEdit *emailField;
    QPushButton *send_code_Button;
    Player      *Players[2];
};
#endif // FORGOT_PASSWORD_H
