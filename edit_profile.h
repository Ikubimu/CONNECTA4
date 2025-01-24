#ifndef EDIT_PROFILE_H
#define EDIT_PROFILE_H
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
#include <QFileDialog>
#include <QDir>
#include <QBuffer>
#include <QImage>
#include <QMouseEvent>
#include <QComboBox>
#include <QScrollArea>
#include "lib/connect4.h"
#include "language.h"
#include "Register.h"

class EditProfilePage : public QDialog
{
    Q_OBJECT

public:
    EditProfilePage(QWidget *parent = nullptr,Player *player = nullptr);
    QString avatarPath;

signals:
    void Edit_Profile_succesful();

private:
    Player *player;
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QLineEdit *emailField;
    QDateEdit *BirthdayField;
    QComboBox *avatarComboBox;
    QImage avatarImage;
    QPushButton *EditProfileButton;
    QLabel *usernameErrorLabel = nullptr;
    QLabel *passwordErrorLabel = nullptr;
    QLabel *emailErrorLabel = nullptr;
    QLabel *birthdayErrorLabel = nullptr;
    QLabel *avatarLabel = nullptr;
    QVBoxLayout *mainLayout;
    QGridLayout *formLayout;
    void handleEditProfile();
    void selectImageFromFile();
};

#endif // LOGIN_H
