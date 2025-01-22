#include "Register.h"
#include "QRegularExpression"

RegisterPage::RegisterPage(QWidget *parent) : QDialog(parent) {
    //create all the QLineEdits
    //username
    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("usuario");
    usernameField->setMinimumWidth(200);
    // password
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("contraseña");
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setMinimumWidth(200);
    // repeat password for safety
    repeatpasswordField = new QLineEdit(this);
    repeatpasswordField->setPlaceholderText("Confirme la contraseña");
    repeatpasswordField->setEchoMode(QLineEdit::Password);
    repeatpasswordField->setMinimumWidth(200);
    // email
    emailField = new QLineEdit(this);
    emailField->setPlaceholderText("Email");
    emailField->setMinimumWidth(200);
    // birthday
    QLabel *birthdayLabel = new QLabel("Birthday:", this);
    BirthdayField = new QDateEdit(this);
    BirthdayField->setCalendarPopup(true);
    BirthdayField->setDate(QDate::currentDate());
    BirthdayField->setMinimumWidth(200);
    QCalendarWidget *calendar = BirthdayField->calendarWidget();
    calendar->setStyleSheet(
        "QCalendarWidget QWidget { font-size: 14px; } "
        "QCalendarWidget QAbstractItemView:enabled { font-size: 14px; padding: 10px; }"
        "QCalendarWidget QToolButton { font-size: 16px; height: 30px; width: 80px; } "
        "QCalendarWidget QToolButton#qt_calendar_yearbutton { min-width: 100px; }"
        "QCalendarWidget QSpinBox { font-size: 16px; min-width: 100px; padding: 5px; } "
        "QCalendarWidget QMenu { font-size: 18px; padding: 10px; }"
        );
    // Avatar selection
    avatarComboBox = new QComboBox(this);
    avatarComboBox->setIconSize(QSize(100, 100));
    avatarComboBox->setFixedWidth(150);
    avatarComboBox->setMaxVisibleItems(3);

    QStringList avatarPaths = {":/fotos_avatar/foto1.png", ":/fotos_avatar/foto2.png",
                               ":/fotos_avatar/foto3.png", ":/fotos_avatar/foto4.png",
                               ":/fotos_avatar/foto5.png", ":/fotos_avatar/foto6.png"};
    //bucle hecho con IA
    for (const QString &path : avatarPaths) {
        QPixmap pixmap(path);
        if (!pixmap.isNull()) {
            avatarComboBox->addItem(QIcon(pixmap), "");
        }
    }

    // Button for custom image selection
    QPushButton *selectImageButton = new QPushButton("Elegir foto del ordenador ", this);
    selectImageButton->setFixedWidth(avatarComboBox->width());
    connect(selectImageButton, &QPushButton::clicked, this, &RegisterPage::selectImageFromFile);

    // Register button
    RegisterButton = new QPushButton("Register", this);
    connect(RegisterButton, &QPushButton::clicked, this, &RegisterPage::handleRegister);

    // Layouts
    mainLayout = new QVBoxLayout(this);
    formLayout = new QGridLayout();
    // Avatar layout (vertical for avatar and button)
    QVBoxLayout *avatarLayout = new QVBoxLayout();
    avatarLayout->addWidget(avatarComboBox);
    avatarLayout->addWidget(selectImageButton);
    // Add fields to form layout
    formLayout->addWidget(usernameField, 0, 1);
    formLayout->addWidget(passwordField, 1, 1);
    formLayout->addWidget(repeatpasswordField, 2, 1);
    formLayout->addWidget(emailField, 4, 1);
    formLayout->addWidget(birthdayLabel, 5, 0);
    formLayout->addWidget(BirthdayField, 5, 1);

    mainLayout->addLayout(avatarLayout);
    mainLayout ->addLayout(formLayout);

    // Add Register button at the bottom of the form
    mainLayout->addWidget(RegisterButton);

    setLayout(mainLayout);
    setWindowTitle("Register Page");
    resize(400, 500);
}

void RegisterPage::selectImageFromFile() {
    // Open file dialog to select an image
    QString filePath = QFileDialog::getOpenFileName(this, "Select Avatar", QDir::homePath(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            // Add the selected image to the ComboBox and select it
            avatarComboBox->addItem(QIcon(pixmap), "");
            avatarComboBox->setCurrentIndex(avatarComboBox->count() - 1);
        } else {
            QMessageBox::warning(this, "Error", "Failed to load the selected image.");
        }
    }
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
    // obtener la imagen seleccionada
    int selectedIndex = avatarComboBox->currentIndex();
    QIcon selectedIcon = avatarComboBox->itemIcon(selectedIndex);
    QPixmap pixmap = selectedIcon.pixmap(100, 100);
    avatarImage = pixmap.toImage();
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

            formLayout->addWidget(usernameErrorLabel, 0, 2);
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
            passwordErrorLabel->setWordWrap(true);
            formLayout->addWidget(passwordErrorLabel, 1, 2);
            passwordField->setStyleSheet("border: 2px solid red;");
        }

        if (!repeat_passwordValid) {
            QString repeatPasswordErrors = "Las contraseñas no coinciden.\n";
            repeatPasswordErrorLabel = new QLabel(repeatPasswordErrors, this);
            repeatPasswordErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            formLayout->addWidget(repeatPasswordErrorLabel, 2, 2);
            repeatpasswordField->setStyleSheet("border: 2px solid red;");
        }

        if (!emailValid) {
            QString emailErrors = "El correo electrónico no es válido.\n";
            emailErrorLabel = new QLabel(emailErrors, this);
            emailErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            formLayout->addWidget(emailErrorLabel, 3, 2);
            emailField->setStyleSheet("border: 2px solid red;");
        }

        if (!birthdayValid) {
            QString birthdayErrors = "Debes de tener más de 12 años para poder registrarte.\n";
            birthdayErrorLabel = new QLabel(birthdayErrors, this);
            birthdayErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            birthdayErrorLabel->setWordWrap(true);
            formLayout->addWidget(birthdayErrorLabel, 4, 2);
            BirthdayField->setStyleSheet("border: 2px solid red;");
        }
        //adaptar tamaño del boton
        return;
    }
    if(existUsername){
      QMessageBox::warning(this, "Error", "Ese usuario ya existe");
        return;
    }
    //Registramos el usuario si todo es valido
    Player* user_player = db.registerPlayer(username, email, password, Birthday, 0,avatarImage);
    if (user_player != nullptr) {
        emit Register_succesful(user_player);
        accept();
    } else {
        // Manejar errores imprevistos
        QMessageBox::warning(this, "Error", "Vuelva a registrarse, hemos tenido un error en la página.");
        accept();
    }
}


