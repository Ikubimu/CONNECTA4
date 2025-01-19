#include "Register.h"
#include "QRegularExpression"
RegisterPage::RegisterPage(QWidget *parent):QDialog(parent){
    //nickname
    QLabel *usernameLabel = new QLabel("Nickname:",this);
    usernameField = new QLineEdit(this);
    //password
    QLabel *passwordLabel = new QLabel("Password:",this);
    passwordField = new QLineEdit(this);
    passwordField -> setEchoMode(QLineEdit::Password);
    //repeat password for safety
    QLabel *repeatpasswordLabel = new QLabel("Repeat Password:",this);
    repeatpasswordField = new QLineEdit(this);
    repeatpasswordField -> setEchoMode(QLineEdit::Password);
    //email
    QLabel *emailLabel = new QLabel("Email:", this);
    emailField = new QLineEdit(this);
    //birthday
    QLabel *birthdayLabel = new QLabel("Birthday:", this);
    BirthdayField = new QDateEdit(this);
    BirthdayField->setCalendarPopup(true); //Se va a mostrar el calendario como ventana emergente
    BirthdayField->setDate(QDate::currentDate()); //se comienza por la fecha del current date

    QCalendarWidget *calendar = BirthdayField->calendarWidget();
    calendar->setStyleSheet(
        "QCalendarWidget QWidget { font-size: 14px; } " // Ajusta el tamaño de texto general
        "QCalendarWidget QAbstractItemView:enabled { font-size: 14px; padding: 10px; }" // Espaciado de los días
        "QCalendarWidget QToolButton { font-size: 16px; height: 30px; width: 80px; } " // Tamaño de los botones
        "QCalendarWidget QToolButton#qt_calendar_yearbutton { min-width: 100px; }" // Aumentar ancho del botón del año
        "QCalendarWidget QSpinBox { font-size: 16px; min-width: 100px; padding: 5px; } " // Ajustar el campo de edición de año
        "QCalendarWidget QMenu { font-size: 18px; padding: 10px; }" // Tamaño del menú desplegable
        );
    //boton para registrarse
    RegisterButton = new QPushButton("Register",this);
    connect(RegisterButton,&QPushButton::clicked,this,&RegisterPage::handleRegister);

    registerLayout = new QGridLayout(this);
    //crear el diseño
    registerLayout->addWidget(usernameLabel,0,0);
    registerLayout->addWidget(usernameField,0,1);
    registerLayout->addWidget(passwordLabel,1,0);
    registerLayout->addWidget(passwordField,1,1);
    registerLayout->addWidget(repeatpasswordLabel,2,0);
    registerLayout->addWidget(repeatpasswordField,2,1);
    registerLayout->addWidget(emailLabel,3,0);
    registerLayout->addWidget(emailField,3,1);
    registerLayout->addWidget(birthdayLabel,4,0);
    registerLayout->addWidget(BirthdayField,4,1);
    registerLayout->addWidget(RegisterButton,7,0,1,2);
    setLayout(registerLayout);
    setWindowTitle("Register Page");
    resize(800,800);
}

bool RegisterPage::check_birthday(int day,int month,int year){
    time_t now = time(0);
    tm *current = localtime(&now);
    int currentDay = current->tm_mday;
    int currentMonth = current->tm_mon + 1; // es 0 para enero
    int currentYear = current->tm_year + 1900; //empieza desde 1900
    int ageYears = currentYear - year;
    if(currentMonth < month || (currentMonth == month && currentDay < day)){
        ageYears--;
    }
    return ageYears >= 12;
}
 bool RegisterPage::check_email(const QString& email){
    //expresion regularExpression
    static QRegularExpression regex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
    QRegularExpressionMatch match = regex.match(email);
    return match.hasMatch();
}
bool RegisterPage::check_username(const QString& username,bool& size, bool& no_spaces){
    size = (username.size() >= 6 && username.size() <= 15);
    no_spaces = !username.contains(' ');
    return size && no_spaces;
}

bool  RegisterPage::check_password(const QString& password,bool& size,bool& mayus,bool& minus,bool& digit,bool& car_special){
    size = password.size() >= 8 && password.size() <= 20;
    QString special_caracters = "!@#$%&*()-+=";
    for(int i = 0; i < password.size(); i++){
        if(password[i] <= 'Z' && password[i] >= 'A'){ //checkear si es mayus
            mayus = true;
        }
        if(password[i] <= 'z' && password[i] >= 'a'){ //checkear si es minus
            minus = true;
        }
        if(password[i] <= '9' && password[i] >= '0'){ //checkear si es digit
            digit = true;
        }
        if(special_caracters.contains(password[i])){
            car_special = true;
        }
    }
    return size && mayus && minus && digit && car_special;
}
bool RegisterPage::compare_password(const QString& password,const QString& repeatpassword){
    return password == repeatpassword;
}
void RegisterPage::handleRegister(){
    //leer los datos del registro
    QString username = usernameField->text();
    QString password = passwordField->text();
    QString repeat_password = repeatpasswordField ->text();
    QString email = emailField->text();
    QDate Birthday = BirthdayField->date();

    // bools para comprobar que esta fallando si algo falla
    bool usernameValid, passwordValid, emailValid, birthdayValid,repeat_passwordValid;
    bool size, spaces, mayus, minus, digit, specialChar;

    size = spaces = mayus = minus = digit = specialChar = false; // we suposse that everything is false and when we check if it's true will change

    usernameValid = check_username(username, size, spaces);
    passwordValid = check_password(password, size, mayus, minus, digit, specialChar);
    emailValid = check_email(email);
    birthdayValid = check_birthday(Birthday.day(), Birthday.month(), Birthday.year());
    repeat_passwordValid = compare_password(password,repeat_password);

    Connect4& db = Connect4::getInstance();
    bool existUsername = db.existsNickName(username);
    //eliminamos todos los errores antes de comenzar y reseteamos el estilo de los campos
    usernameField->setStyleSheet("");
    passwordField->setStyleSheet("");
    repeatpasswordField->setStyleSheet("");
    emailField->setStyleSheet("");
    BirthdayField->setStyleSheet("");
    if (usernameErrorLabel) {
        delete usernameErrorLabel;
        usernameErrorLabel = nullptr;
    }
    if (passwordErrorLabel) {
        delete passwordErrorLabel;
        passwordErrorLabel = nullptr;
    }
    if (repeatPasswordErrorLabel) {
        delete repeatPasswordErrorLabel;
        repeatPasswordErrorLabel = nullptr;
    }
    if (emailErrorLabel) {
        delete emailErrorLabel;
        emailErrorLabel = nullptr;
    }
    if (birthdayErrorLabel) {
        delete birthdayErrorLabel;
        birthdayErrorLabel = nullptr;
    }

    // Comprobación por si hay algun error
    if (!usernameValid || !passwordValid || !emailValid || !birthdayValid || !repeat_passwordValid) {
        if (!usernameValid) {
            QString usernameErrors;
            if (!usernameValid) {
                if (!size) usernameErrors += "Debe tener entre 6 y 15 caracteres.\n";
                if (!spaces) usernameErrors += "No debe contener espacios.\n";
            }
            usernameErrorLabel = new QLabel(usernameErrors, this);

            registerLayout->addWidget(usernameErrorLabel, 0, 2);
            usernameErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            usernameField->setStyleSheet("border: 2px solid red;");
        }
        if (!passwordValid) {
            QString passwordErrors;
            if (!size) passwordErrors += "Debe tener entre 8 y 20 caracteres.\n";
            if (!mayus) passwordErrors += "Debe incluir al menos una mayúscula.\n";
            if (!minus) passwordErrors += "Debe incluir al menos una minúscula.\n";
            if (!digit) passwordErrors += "Debe incluir al menos un número.\n";
            if (!specialChar) passwordErrors += "Debe incluir al menos un carácter especial.\n";

            passwordErrorLabel = new QLabel(passwordErrors, this);
            passwordErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            registerLayout->addWidget(passwordErrorLabel, 1, 2);
            passwordField->setStyleSheet("border: 2px solid red;");
        }

        if (!repeat_passwordValid) {
            QString repeatPasswordErrors = "Las contraseñas no coinciden.\n";
            repeatPasswordErrorLabel = new QLabel(repeatPasswordErrors, this);
            repeatPasswordErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            registerLayout->addWidget(repeatPasswordErrorLabel, 2, 2);
            repeatpasswordField->setStyleSheet("border: 2px solid red;");
        }

        if (!emailValid) {
            QString emailErrors = "El correo electrónico no es válido.\n";
            emailErrorLabel = new QLabel(emailErrors, this);
            emailErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            registerLayout->addWidget(emailErrorLabel, 3, 2);
            emailField->setStyleSheet("border: 2px solid red;");
        }

        if (!birthdayValid) {
            QString birthdayErrors = "Debes de tener más de 12 años para poder registrarte.\n";
            birthdayErrorLabel = new QLabel(birthdayErrors, this);
            birthdayErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            registerLayout->addWidget(birthdayErrorLabel, 4, 2);
            BirthdayField->setStyleSheet("border: 2px solid red;");
        }
        //adaptar tamaño del boton
        registerLayout->addWidget(RegisterButton,7,0,1,3);
        return;
    }
    if(existUsername){
      QMessageBox::warning(this, "Error", "Ese usuario ya existe");
        return;
    }
    //Registramos el usuario si todo es valido
    Player* user_player = db.registerPlayer(username, email, password, Birthday, 0);
    if (user_player != nullptr) {
        emit Register_succesful(user_player);
        accept();
    } else {
        // Manejar errores imprevistos
        QMessageBox::warning(this, "Error", "Vuelva a registrarse, hemos tenido un error en la página.");
        accept();
    }
}

