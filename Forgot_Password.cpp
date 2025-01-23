#include "Forgot_Password.h"

ForgotPasswordPage::ForgotPasswordPage(QWidget *parent, Player* players_playing[2])
    : QDialog(parent)
{
    // Copiar el puntero de los jugadores
    Players[0] = players_playing[0];
    Players[1] = players_playing[1];

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText(Labels::username);
    emailField = new QLineEdit(this);
    emailField->setPlaceholderText(Labels::email);

    send_code_Button = new QPushButton(Labels::Send_code2email, this);
    connect(send_code_Button, &QPushButton::clicked, this, &ForgotPasswordPage::handleForgot);

    layout = new QGridLayout(this);
    layout->addWidget(usernameField, 0, 0);
    layout->addWidget(emailField, 1, 0);
    layout->addWidget(send_code_Button, 2, 0);

    setLayout(layout);
    setWindowTitle(Labels::forgot_password);
    resize(400,200);

}
void ForgotPasswordPage::handleForgot()
{
    QString username = usernameField->text();
    QString email = emailField->text();

    Connect4& db = Connect4::getInstance();
    bool existUsername = db.existsNickName(username);
    if(!existUsername){
        QMessageBox::warning(this, Labels::error,Labels:: non_user);
        return;
    }
    Player *player = db.getPlayer(username);
    if(player == Players[0] || player == Players[1]){
        QMessageBox::warning(this,Labels::error,Labels::currently_logged);
        this->close();
        return;
    }
    if(player->getEmail() != email){
        QMessageBox::warning(this, Labels::error,Labels::wrong_email);
        return;
    }
    srand(static_cast<unsigned>(time(0)));
    int code = rand() % 10000; // Creamos un numero de 4 números como código
    QMessageBox::information(this, Labels::code, QString("Tu código es: %1").arg(code, 4, 10, QChar('0')));

    //cambiamos total el diseño para que se inserte el código de recuperación
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QLineEdit *codeField = new QLineEdit(this);
    codeField->setPlaceholderText(Labels::insert_code);
    QPushButton *verifyButton = new QPushButton(Labels::verify_code, this);

    layout->addWidget(new QLabel(Labels::code2verify, this), 0, 0);
    layout->addWidget(codeField, 0, 1);
    layout->addWidget(verifyButton, 1, 0, 1, 2);

    // Conectar el botón de verificación a una función lambda
    connect(verifyButton, &QPushButton::clicked, this, [this, codeField, code,player]() {
        if (codeField->text().toInt() != code) {
            QMessageBox::warning(this, Labels::error, Labels::wrong_code);
            return;
        }
        emit Login_succesful(player);
        this->close();
    });
}

