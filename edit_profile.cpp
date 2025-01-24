#include "edit_profile.h"
#include "QRegularExpression"

EditProfilePage::EditProfilePage(QWidget *parent,Player *player_to_edit) : QDialog(parent) {
    //create all the QLineEdits
    player = player_to_edit;

    //username
    QLabel *usernameLabel = new QLabel(Labels::username + ":",this);
    usernameField = new QLineEdit(this);
    usernameField->setText(player->getNickName());
    usernameField->setReadOnly(true);
    usernameField->setMinimumWidth(200);
    // password
    QLabel *passwordLabel = new QLabel(Labels::password + ":", this);
    passwordField = new QLineEdit(this);
    passwordField->setText(player->getPassword());
    passwordField->setMinimumWidth(200);
    // email
    QLabel *emailLabel = new QLabel(Labels::email + ":", this);
    emailField = new QLineEdit(this);
    emailField->setText(player->getEmail());
    emailField->setMinimumWidth(200);
    // birthday
    QLabel *birthdayLabel = new QLabel(Labels::birthday + ":", this);
    BirthdayField = new QDateEdit(this);
    BirthdayField->setCalendarPopup(true);
    BirthdayField->setDate(player->getBirthdate()); // Establecer la fecha del jugador
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
    // Seleccionar el avatar del usuario en la lista
    QPixmap currentAvatar = QPixmap::fromImage(player->getAvatar());
    for (int i = 0; i < avatarComboBox->count(); ++i) {
        if (!avatarComboBox->itemIcon(i).pixmap(100, 100).toImage().isNull() &&
            avatarComboBox->itemIcon(i).pixmap(100, 100).toImage() == currentAvatar.toImage()) {
            avatarComboBox->setCurrentIndex(i);
            break;
        }
    }
    // Button for custom image selection
    QPushButton *selectImageButton = new QPushButton(Labels::choose_photo_pc, this);
    selectImageButton->setFixedWidth(avatarComboBox->width());
    connect(selectImageButton, &QPushButton::clicked, this, &EditProfilePage::selectImageFromFile);

    // Register button
    EditProfileButton = new QPushButton(Labels::confirm_edit_profile, this);
    connect(EditProfileButton, &QPushButton::clicked, this, EditProfilePage::handleEditProfile);

    // Layouts
    mainLayout = new QVBoxLayout(this);
    formLayout = new QGridLayout();
    // Avatar layout (vertical for avatar and button)
    QVBoxLayout *avatarLayout = new QVBoxLayout();
    avatarLayout->addWidget(avatarComboBox);
    avatarLayout->addWidget(selectImageButton);
    // Add fields to form layout
    formLayout->addWidget(usernameLabel, 0, 0);
    formLayout->addWidget(usernameField, 0, 1);
    formLayout->addWidget(passwordLabel, 1, 0);
    formLayout->addWidget(passwordField, 1, 1);
    formLayout->addWidget(emailLabel, 3, 0);
    formLayout->addWidget(emailField, 3, 1);
    formLayout->addWidget(birthdayLabel, 4, 0);
    formLayout->addWidget(BirthdayField, 4, 1);

    mainLayout->addLayout(avatarLayout);
    mainLayout ->addLayout(formLayout);

    // Add Register button at the bottom of the form
    mainLayout->addWidget(EditProfileButton);

    setLayout(mainLayout);
    setWindowTitle(Labels::edit_profile);
    resize(400, 500);
}

void EditProfilePage::selectImageFromFile() {
    // Open file dialog to select an image
    QString filePath = QFileDialog::getOpenFileName(this, Labels::select_avatar, QDir::homePath(), "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            // Add the selected image to the ComboBox and select it
            avatarComboBox->addItem(QIcon(pixmap), "");
            avatarComboBox->setCurrentIndex(avatarComboBox->count() - 1);
        } else {
            QMessageBox::warning(this, Labels::error,Labels::fail_uploading_image);
        }
    }
}

void EditProfilePage::handleEditProfile(){
    //leer los datos del registro
    QString password = passwordField->text();
    QString email = emailField->text();
    QDate Birthday = BirthdayField->date();
    // obtener la imagen seleccionada
    int selectedIndex = avatarComboBox->currentIndex();
    QIcon selectedIcon = avatarComboBox->itemIcon(selectedIndex);
    QPixmap pixmap = selectedIcon.pixmap(100, 100);
    avatarImage = pixmap.toImage();
    // bools para comprobar que esta fallando si algo falla
    bool passwordValid, emailValid, birthdayValid;
    bool size_password, spaces, mayus, minus, digit, specialChar;

    size_password = spaces = mayus = minus = digit = specialChar = false; // we suposse that everything is false and when we check if it's true will change


    passwordValid = RegisterPage::check_password(password, size_password, mayus, minus, digit, specialChar);
    emailValid = RegisterPage::check_email(email);
    birthdayValid = RegisterPage::check_birthday(Birthday.day(), Birthday.month(), Birthday.year());

    usernameField->setStyleSheet("");
    passwordField->setStyleSheet("");
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
    if (emailErrorLabel) {
        delete emailErrorLabel;
        emailErrorLabel = nullptr;
    }
    if (birthdayErrorLabel) {
        delete birthdayErrorLabel;
        birthdayErrorLabel = nullptr;
    }

    // Comprobación por si hay algun error
    if (!passwordValid || !emailValid || !birthdayValid) {
        if (!passwordValid) {
            QString passwordErrors;
            if (!size_password) passwordErrors += Labels::password_restriccion_character;
            if (!mayus) passwordErrors += Labels::password_mayus;
            if (!minus) passwordErrors += Labels::password_minus;
            if (!digit) passwordErrors +=Labels::password_number;
            if (!specialChar) passwordErrors += Labels::password_special;

            passwordErrorLabel = new QLabel(passwordErrors, this);
            passwordErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            passwordErrorLabel->setWordWrap(true);
            formLayout->addWidget(passwordErrorLabel, 1, 2);
            passwordField->setStyleSheet("border: 2px solid red;");
        }

        if (!emailValid) {
            QString emailErrors = Labels::nonvalid_email;
            emailErrorLabel = new QLabel(emailErrors, this);
            emailErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            formLayout->addWidget(emailErrorLabel, 3, 2);
            emailField->setStyleSheet("border: 2px solid red;");
        }

        if (!birthdayValid) {
            QString birthdayErrors = Labels::wrong_birthday;
            birthdayErrorLabel = new QLabel(birthdayErrors, this);
            birthdayErrorLabel->setStyleSheet("color: red; font-size: 12px;");
            birthdayErrorLabel->setWordWrap(true);
            formLayout->addWidget(birthdayErrorLabel, 4, 2);
            BirthdayField->setStyleSheet("border: 2px solid red;");
        }
        return;
    }
    //Editamos los campos del usuario si todo es valido
    player->setAvatar(avatarImage);
    player->setBirthdate(Birthday);
    player->setEmail(email);
    player->setPassword(password);

    emit Edit_Profile_succesful();
    accept();
}


