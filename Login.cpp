#include "Login.h"
LoginPage::LoginPage(QWidget *parent,Player* players_playing[2]) : QDialog(parent) {
    // Crear los elementos de la UI
    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("usuario");

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("contraseña");
    passwordField->setEchoMode(QLineEdit::Password); // Ocultar la contraseña

    loginButton = new QPushButton(Labels::login, this);
    RegisterButton = new QPushButton(Labels::signup_newacc, this);
    ForgetPasswordButton = new QPushButton(Labels::forgot_password_dude,this);
    // Conectar los botones
    connect(loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(RegisterButton, &QPushButton::clicked, this, &LoginPage::change_to_register);
    connect(ForgetPasswordButton,&QPushButton::clicked,this,&LoginPage::change_to_forgot_password);
    // Crear el layout
    layout = new QGridLayout(this);
    layout->addWidget(usernameField,0,0,1,2);
    layout->addWidget(passwordField,1,0,1,2);
    layout->addWidget(loginButton,3,0,1,2);
    layout ->addWidget(ForgetPasswordButton,4,0,1,2);
    layout->addWidget(RegisterButton,5,0,1,2);

    //add players so after you can check if the user has already login
    Players[0] = players_playing[0];
    Players[1] = players_playing[1];
    setLayout(layout);
    setWindowTitle(tr("Inicio de sesión"));
    resize(500, 500);
}
void LoginPage::handleLogin(){
    QString username = usernameField->text();
    QString password = passwordField->text();

    // bools para comprobar que esta fallando si algo falla
    bool usernameValid, passwordValid;
    bool size_user, spaces, mayus, minus, digit, specialChar,size_password;

    size_user = size_password = spaces = mayus = minus = digit = specialChar = false; // we suposse that everything is false and when we check if it's true will change

    usernameValid = RegisterPage::check_username(username, size_user, spaces);
    passwordValid = RegisterPage::check_password(password, size_password, mayus, minus, digit, specialChar);

    // Eliminamos cualquier error que hayamos puesto antes y reseteamos los campos
    usernameField->setStyleSheet("");
    passwordField->setStyleSheet("");

    if (usernameErrorLabel) {
        delete usernameErrorLabel;
        usernameErrorLabel = nullptr;
    }
    if (passwordErrorLabel) {
        delete passwordErrorLabel;
        passwordErrorLabel = nullptr;
    }

    if (!usernameValid || !passwordValid) {
        if (!usernameValid) {
            QString usernameErrors;
            if (!size_user) usernameErrors += Labels::user_restriccion_character;
            if (spaces) usernameErrors += Labels::no_spacer;

            usernameErrorLabel = new QLabel(usernameErrors, this);
            layout->addWidget(usernameErrorLabel, 0, 2);
            // esto habrá que cambiarlo al css
            usernameErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            usernameField->setStyleSheet("border: 2px solid red;"); // Resaltar en rojo
        }

        if (!passwordValid) {
            QString passwordErrors;
            if (!size_password) passwordErrors += Labels::password_restriccion_character;
            if (!mayus) passwordErrors += Labels::password_mayus;
            if (!minus) passwordErrors += Labels::password_minus;
            if (!digit) passwordErrors += Labels::password_number;
            if (!specialChar) passwordErrors += Labels::password_special;

            passwordErrorLabel = new QLabel(passwordErrors, this);
            passwordErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            layout->addWidget(passwordErrorLabel, 1, 2); // Colocar al lado del campo de texto
            passwordField->setStyleSheet("border: 2px solid red;"); // Resaltar en rojo
        }
        layout->addWidget(loginButton,4,0,1,3);
        layout->addWidget(ForgetPasswordButton,5,0,1,3);
        layout->addWidget(RegisterButton,6,0,1,3);
        return;
    }
    // Todo valido proceder con el login
    Connect4& db = Connect4::getInstance();
    Player* user_player = db.loginPlayer(username, password);
    if (user_player == nullptr) {
        QMessageBox::warning(this, Labels::error, Labels::non_user);
        return;
    }
    if (user_player == Players[0]) {
        QMessageBox::warning(this, Labels::error, Labels::user_already_logged);
        return;
    }
    emit Login_succesful(user_player);
    accept();
}
void LoginPage::change_to_register(){
    this->close();
    emit requestRegisterPage();
}
void LoginPage::change_to_forgot_password(){
    this->close();
    emit requestForgotPasswordPage();
}
