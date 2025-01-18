#ifndef REGISTER_H
#define REGISTER_H
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <ctime>
#include <QDialog>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QString>
#include <QMessageBox>
#include "lib/connect4.h"

class RegisterPage : public QDialog
{
    Q_OBJECT

public:
    RegisterPage(QWidget *parent = nullptr);
    //methods to check the differents fields
   static bool check_username(const QString& username,bool& size,bool& spaces);
   static bool check_password(const QString& password,bool& size,bool& mayus,bool& minus,bool& digit,bool& car_special);
   static bool check_birthday(int day,int month,int year);
   static bool check_email(const QString& email);
   static bool compare_password(const QString& password,const QString& repeatpassword);

signals:
    void Register_succesful(Player* player);

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QLineEdit *repeatpasswordField;
    QLineEdit *emailField;
    QDateEdit *BirthdayField;
    QPushButton *RegisterButton;
    QLabel *usernameErrorLabel = nullptr;
    QLabel *passwordErrorLabel = nullptr;
    QLabel *emailErrorLabel = nullptr;
    QLabel *birthdayErrorLabel = nullptr;
    QLabel *repeatPasswordErrorLabel = nullptr;
    QGridLayout *registerLayout;
    void handleRegister(); //slot
};

#endif // LOGIN_H
