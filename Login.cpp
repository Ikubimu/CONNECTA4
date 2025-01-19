#include "Login.h"
LoginPage::LoginPage(QWidget *parent,Player* players_playing[2]) : QDialog(parent) {
    // Crear los elementos de la UI
    QLabel *usernameLabel = new QLabel("Nombre de usuario:", this);
    usernameField = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("Contraseña:", this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password); // Ocultar la contraseña

    loginButton = new QPushButton("Iniciar sesión", this);
    RegisterButton = new QPushButton("Registrar una nueva cuenta", this);
    ForgetPasswordButton = new QPushButton("¿Olvidaste la contraseña?",this);
    // Conectar los botones
    connect(loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(RegisterButton, &QPushButton::clicked, this, &LoginPage::change_to_register);
    connect(ForgetPasswordButton,&QPushButton::clicked,this,&LoginPage::change_to_forgot_password);
    // Crear el layout
    layout = new QGridLayout(this);
    layout->addWidget(usernameLabel,0,0);
    layout->addWidget(usernameField,0,1);
    layout->addWidget(passwordLabel,1,0);
    layout->addWidget(passwordField,1,1);
    layout->addWidget(loginButton,4,0,1,2);
    layout ->addWidget(ForgetPasswordButton,5,0,1,2);
    layout->addWidget(RegisterButton,6,0,1,2);

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
    bool size, spaces, mayus, minus, digit, specialChar;

    size = spaces = mayus = minus = digit = specialChar = false; // we suposse that everything is false and when we check if it's true will change

    usernameValid = RegisterPage::check_username(username, size, spaces);
    passwordValid = RegisterPage::check_password(password, size, mayus, minus, digit, specialChar);

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
            if (!size) usernameErrors += "Debe tener entre 6 y 15 caracteres.\n";
            if (spaces) usernameErrors += "No debe contener espacios.\n";

            usernameErrorLabel = new QLabel(usernameErrors, this);
            layout->addWidget(usernameErrorLabel, 0, 2);
            // esto habrá que cambiarlo al css
            usernameErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            usernameField->setStyleSheet("border: 2px solid red;"); // Resaltar en rojo
        }

        if (!passwordValid) {
            QString passwordErrors;
            if (!size) passwordErrors += "Debe tener entre 8 y 20 caracteres.\n";
            if (!mayus) passwordErrors += "Debe incluir al menos una mayúscula.\n";
            if (!minus) passwordErrors += "Debe incluir al menos una minúscula.\n";
            if (!digit) passwordErrors += "Debe incluir al menos un número.\n";
            if (!specialChar) passwordErrors += "Debe incluir al menos un carácter especial:!@#$%&*()-+=.\n";

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
        QMessageBox::warning(this, "Error", "El usuario no existe.");
        return;
    }
    if (user_player == Players[0]) {
        QMessageBox::warning(this, "Error", "El usuario ya está loggeado.");
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
