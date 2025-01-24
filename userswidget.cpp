#include "userswidget.h"
#define CIRCLE 50
usersWidget::usersWidget(QWidget *parent): QWidget(parent),actual(No_players){
    players_playing[0] = nullptr;
    Connect4& game = Connect4::getInstance();
    robot = game.registerPlayer("ROBOT", "robot@robot.com", "Password123!", QDate(1990, 1, 1), 0);
    players_playing[1] = robot;

    mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    updateWidget(No_players);

}

void usersWidget::updateWidget(actual_situation x){
    actual = x;
    // Limpiar el layout principal
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
    switch (x) {
    case No_players:
        setupNoPlayersWidget();
        break;
    case One_player:
        setupOnePlayerWidget();
        break;
    case Two_players:
        setupTwoPlayersWidget();
        break;
    }
}
start_player usersWidget::get_who_starts(){
    int index = whoStarts->currentIndex();
    switch (index){
        case 0: return start_player::first_player;
        case 1: return start_player::second_player;
        case 2: return start_player::random;
    }
    return start_player::random;
}
void usersWidget::openLoginPage()
{
    LoginPage loginDialog(nullptr,players_playing);
    connect(&loginDialog, &LoginPage::Login_succesful, this, &usersWidget::handleLoginSuccess);
    connect(&loginDialog, &LoginPage::requestRegisterPage, this, &usersWidget::openRegisterPage);
    connect(&loginDialog,&LoginPage::requestForgotPasswordPage,this,&usersWidget::openForgotPasswordPage);
    loginDialog.exec();
}
QPixmap usersWidget::createCircularPixmap(const QImage &image)
{
    QPixmap pixmap = QPixmap::fromImage(image).scaled(CIRCLE, CIRCLE, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QBitmap mask(CIRCLE, CIRCLE);
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setBrush(Qt::color1);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawEllipse(0, 0, CIRCLE, CIRCLE);
    painter.end();
    pixmap.setMask(mask);
    return pixmap;
}
void usersWidget::openRegisterPage(){
    RegisterPage RegisterDialog;
    //conectada a la misma funcion ya que al final para el userwidget sigue siendo que alguien sea loggeado
    //sea por register o por loggin
    connect(&RegisterDialog, &RegisterPage::Register_succesful, this, &usersWidget::handleLoginSuccess);
    RegisterDialog.exec();
}

void usersWidget::handleLoginSuccess(Player *player){
    //two possible situations
    if(actual == actual_situation::No_players){
        actual = actual_situation::One_player;
        players_playing[0] = player;
        updateWidget(actual);
    }else{ // actual situation deberia de estar en dos players
        if(players_playing[0]->getNickName() == player->getNickName()){
            QMessageBox::warning(this, "Error", "Este jugador ya ha iniciado sesión.", QMessageBox::Ok);
            return;
        }

        actual = actual_situation::Two_players;
        players_playing[1] = player;
        updateWidget(actual);
    }
}

void usersWidget::openForgotPasswordPage(){
    ForgotPasswordPage ForgotPasswordDialog(nullptr,players_playing);
    connect(&ForgotPasswordDialog,&ForgotPasswordPage::Login_succesful,this,&usersWidget::handleLoginSuccess);
    ForgotPasswordDialog.exec();
}
void usersWidget::openConfigureProfile(){
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    if (buttonSender == leftEditprofile) {

    } else if (buttonSender == rightEditprofile) {

    }
}
void usersWidget::log_out(){
    QMessageBox confirmDialog(this);
    confirmDialog.setWindowTitle("Confirmar cierre de sesión");
    confirmDialog.setText("¿Seguro que deseas cerrar sesión?");
    confirmDialog.setIcon(QMessageBox::Question);
    confirmDialog.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    confirmDialog.setDefaultButton(QMessageBox::No); // Por defecto, selecciona "No"

    int ret = confirmDialog.exec(); // captura la respuesta del usuario

    if(ret == QMessageBox::No) return;
    //si le ha dado a si lo eliminamos
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    if (buttonSender == leftlog_out) {
        if(actual == actual_situation::Two_players){
            players_playing[0] = players_playing[1]; // cambiamos el players_playing1 a la izquierda
            actual = actual_situation::One_player;
            updateWidget(actual);
        }else{
            players_playing[0] = nullptr;
            actual = actual_situation::No_players;
            updateWidget(actual);
        }
    }else if (buttonSender == rightlog_out) {
        actual = actual_situation::One_player;
        updateWidget(actual);
    }
}

int usersWidget::get_players(Player* players[2]){
    if(players_playing[0] == nullptr){
        return 0;
    }
    players[0] = players_playing[0];
    players[1] = players_playing[1];
    return (players_playing[1] == robot) ? 1 : 2;
}

void usersWidget::setupNoPlayersWidget() {
    QWidget *noPlayersWidget = new QWidget(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    QLabel *leftAvatarLabel = new QLabel();
    QImage AvatarLeft = QImage(":/fotos_avatar/noUserImage.jpg");
    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    leftAvatarLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftAvatarLabel);

    leftButtonLogin = new QPushButton("Iniciar Sesión", this);
    connect(leftButtonLogin, &QPushButton::clicked, this, &usersWidget::openLoginPage);
    leftLayout->addWidget(leftButtonLogin);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    QLabel *rightAvatarLabel = new QLabel();
    QImage AvatarRight = QImage(":/fotos_avatar/robot.png");
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightAvatarLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightAvatarLabel);

    QLabel *rightTextLabel = new QLabel("Robot", this);
    rightTextLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightTextLabel);

    //crear el middle widget
    QWidget *middleContainer = new QWidget(this);
    whoStarts = new QComboBox(this);
    whoStarts ->addItem("Jugador 1");
    whoStarts ->addItem("Jugador 2");
    whoStarts ->addItem("Aleatorio");
    whoStarts->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    middleLayout->addWidget(new QLabel("    VS   ", this));
    middleLayout->addWidget(new QLabel("Empieza:",this));
    middleLayout->addWidget(whoStarts);
    middleContainer->setLayout(middleLayout);
    middleContainer->setFixedWidth(100);

    QHBoxLayout *mainLayoutWidget = new QHBoxLayout(noPlayersWidget);
    mainLayoutWidget->addLayout(leftLayout);
    mainLayoutWidget->addWidget(middleContainer);
    mainLayoutWidget->addLayout(rightLayout);

    noPlayersWidget->setLayout(mainLayoutWidget);
    mainLayout->addWidget(noPlayersWidget);
}

void usersWidget::setupOnePlayerWidget() {
    QWidget *onePlayerWidget = new QWidget(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    QLabel *leftAvatarLabel = new QLabel();
    QImage AvatarLeft = players_playing[0]->getAvatar();
    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    leftAvatarLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftAvatarLabel);

    QString nick = players_playing[0]->getNickName();
    leftLayout->addWidget(new QLabel(nick,this));
    leftEditprofile = new QPushButton("Editar Perfil", this);
    connect(leftEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    leftLayout->addWidget(leftEditprofile);

    leftlog_out = new QPushButton("Cerrar sesión", this);
    connect(leftlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    leftLayout->addWidget(leftlog_out);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    QLabel *rightAvatarLabel = new QLabel();
    QImage AvatarRight = QImage(":/fotos_avatar/robot.png");
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightAvatarLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightAvatarLabel);

    QLabel *rightTextLabel = new QLabel("Robot", this);
    rightTextLabel->setAlignment(Qt::AlignCenter);
    rightLayout ->addWidget(rightTextLabel);
    rightButtonLogin = new QPushButton("Jugar con un amigo", this);
    connect(rightButtonLogin, &QPushButton::clicked, this, &usersWidget::openLoginPage);
    rightLayout->addWidget(rightButtonLogin);


    //crear el middle widget
    QWidget *middleContainer = new QWidget(this);
    whoStarts = new QComboBox(this);
    whoStarts ->addItem("Jugador 1");
    whoStarts ->addItem("Jugador 2");
    whoStarts ->addItem("Aleatorio");
    whoStarts->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    middleLayout->addWidget(new QLabel("    VS   ", this));
    middleLayout->addWidget(new QLabel("Empieza",this));
    middleLayout->addWidget(whoStarts);
    middleContainer->setLayout(middleLayout);
    middleContainer->setFixedWidth(100);

    QHBoxLayout *mainLayoutWidget = new QHBoxLayout(onePlayerWidget);
    mainLayoutWidget->addLayout(leftLayout);
    mainLayoutWidget->addWidget(middleContainer);
    mainLayoutWidget->addLayout(rightLayout);

    onePlayerWidget->setLayout(mainLayoutWidget);
    mainLayout->addWidget(onePlayerWidget);
}

void usersWidget::setupTwoPlayersWidget() {
    QWidget *twoPlayersWidget = new QWidget(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    QLabel *leftAvatarLabel = new QLabel();
    QImage AvatarLeft = players_playing[0]->getAvatar();
    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    leftAvatarLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftAvatarLabel);

    QString nick = players_playing[0]->getNickName();
    leftLayout->addWidget(new QLabel(nick,this));
    leftEditprofile = new QPushButton("Editar Perfil", this);
    connect(leftEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    leftLayout->addWidget(leftEditprofile);

    leftlog_out = new QPushButton("Cerrar sesión", this);
    connect(leftlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    leftLayout->addWidget(leftlog_out);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    QLabel *rightAvatarLabel = new QLabel();
    QImage AvatarRight = players_playing[1]->getAvatar();
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightAvatarLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightAvatarLabel);

    QString nick2 = players_playing[1]->getNickName();
    rightLayout->addWidget(new QLabel(nick2,this));

    rightEditprofile = new QPushButton("Editar Perfil", this);
    connect(rightEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    rightLayout->addWidget(rightEditprofile);

    rightlog_out = new QPushButton("Cerrar sesión", this);
    connect(rightlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    rightLayout->addWidget(rightlog_out);

    //crear el middle widget
    QWidget *middleContainer = new QWidget(this);
    whoStarts = new QComboBox(this);
    whoStarts ->addItem("Jugador 1");
    whoStarts ->addItem("Jugador 2");
    whoStarts ->addItem("Aleatorio");
    whoStarts->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    middleLayout->addWidget(new QLabel("    VS   ", this));
    middleLayout->addWidget(new QLabel("¿Quien empieza?",this));
    middleLayout->addWidget(whoStarts);
    middleContainer->setLayout(middleLayout);
    middleContainer->setFixedWidth(120);

    QHBoxLayout *mainLayoutWidget = new QHBoxLayout(twoPlayersWidget);
    mainLayoutWidget->addLayout(leftLayout);
    mainLayoutWidget->addWidget(middleContainer);
    mainLayoutWidget->addLayout(rightLayout);

    twoPlayersWidget->setLayout(mainLayoutWidget);
    mainLayout->addWidget(twoPlayersWidget);
}



