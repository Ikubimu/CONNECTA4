#include "userswidget.h"
#define CIRCLE 50
usersWidget::usersWidget(QWidget *parent): QWidget(parent),actual(No_players){
    actual = actual_situation::No_players;
    players_playing[0] = nullptr;
    Connect4& game = Connect4::getInstance();
    robot = game.registerPlayer("ROBOT", "robot@robot.com", "Password123!", QDate(1990, 1, 1), 0);
    players_playing[1] = robot;

    //Initialize all the elements depends of the situation we will use them or not
    whoStarts = new QComboBox(this);
    whoStarts ->addItem("Jugador 1");
    whoStarts ->addItem("Jugador 2");
    whoStarts ->addItem("Aleatorio");

    Difficulty_Level_Robot = new QComboBox(this);
    Difficulty_Level_Robot->addItem("Facil");
    Difficulty_Level_Robot->addItem("Dificil");

    rightButtonLogin = new QPushButton("Jugar con un amigo", this);
    rightEditprofile = new QPushButton("Editar el Perfil", this);
    rightlog_out = new QPushButton("Cerrar Sesión", this);
    connect(rightButtonLogin, &QPushButton::clicked, this, &usersWidget::openLoginPage);
    connect(rightEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    connect(rightlog_out, &QPushButton::clicked, this, &usersWidget::log_out);

    leftButtonLogin = new QPushButton("Iniciar Sesión", this);
    leftEditprofile = new QPushButton("Editar Perfil", this);
    leftlog_out = new QPushButton("Cerrar sesion", this);
    connect(leftEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    connect(leftButtonLogin, &QPushButton::clicked, this, &usersWidget::openLoginPage);
    connect(leftlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    //initialize middle layout
    middle = new QVBoxLayout(this);
    middle->addWidget(new QLabel("    VS   ",this));
    middle->addWidget(whoStarts);

    stackedWidget = new QStackedWidget(this);
    setupWidgets();
    updateWidget(No_players);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

}


void usersWidget::setupWidgets(){
    setupNoPlayersWidget();
    setupOnePlayerWidget();
    setupTwoPlayersWidget();

    //añadir las paginas
    stackedWidget->addWidget(noPlayersWidget);
    stackedWidget->addWidget(onePlayerWidget);
    stackedWidget->addWidget(twoPlayersWidget);
}
void usersWidget::updateWidget(actual_situation x){
    actual = x;
    switch (x) {
        case No_players:
            stackedWidget->setCurrentWidget(noPlayersWidget);
            break;
        case One_player:
            if (players_playing[0]) {
                AvatarLeft = players_playing[0]->getAvatar();
                leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
                leftAvatarLabel->show();
            }
            stackedWidget->setCurrentWidget(onePlayerWidget);
            break;
        case Two_players:
            if (players_playing[0] && players_playing[1]) {
                AvatarLeft = players_playing[0]->getAvatar();
                AvatarRight = players_playing[1]->getAvatar();
                leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
                rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
            }
            stackedWidget->setCurrentWidget(twoPlayersWidget);
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
    if(players_playing[1] == nullptr){
        return 0;
    }
    players[0] = players_playing[0];
    players[1] = players_playing[1];
    return (players_playing[1] == robot) ? 1 : 2;
}
void usersWidget::setupNoPlayersWidget(){
    noPlayersWidget = new QWidget(this);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftAvatarLabel = new QLabel();
    AvatarLeft = QImage(":/fotos_avatar/noUserImage.jpg");
    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    leftLayout->addWidget(leftAvatarLabel);
    leftLayout->addWidget(leftButtonLogin);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightAvatarLabel = new QLabel();
    AvatarRight = QImage(":/fotos_avatar/robot.png");
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightLayout->addWidget(rightAvatarLabel);

    rightLayout->addWidget(new QLabel("Robot"));
    rightLayout->addWidget(Difficulty_Level_Robot);

    QWidget* middleContainer = new QWidget(this);
    middleContainer->setLayout(middle);
    middleContainer->setFixedWidth(100);
    middle->setAlignment(Qt::AlignCenter);

    // Layout principal horizontal
    QHBoxLayout* mainLayout = new QHBoxLayout(noPlayersWidget);
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(middleContainer);
    mainLayout->addLayout(rightLayout);

    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 2);


}
void usersWidget::setupOnePlayerWidget(){
    onePlayerWidget = new QWidget(this);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftAvatarLabel = new QLabel();
    leftLayout->addWidget(leftAvatarLabel);

    leftLayout->addWidget(leftEditprofile);
    leftLayout->addWidget(leftlog_out);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightAvatarLabel = new QLabel();
    AvatarRight = QImage(":/fotos_avatar/robot.png");
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightLayout->addWidget(rightAvatarLabel);

    rightLayout->addWidget(new QLabel("Robot"));
    rightLayout->addWidget(rightButtonLogin);
    rightLayout->addWidget(Difficulty_Level_Robot);


    QHBoxLayout* mainLayout = new QHBoxLayout(onePlayerWidget);
    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 2);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(middle);
    mainLayout->addLayout(rightLayout);

}
void usersWidget::setupTwoPlayersWidget(){
    twoPlayersWidget = new QWidget(this);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftAvatarLabel = new QLabel();
    leftLayout->addWidget(leftAvatarLabel);

    leftLayout->addWidget(leftEditprofile);
    leftLayout->addWidget(leftlog_out);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightAvatarLabel = new QLabel();
    rightLayout->addWidget(rightAvatarLabel);

    rightLayout->addWidget(rightEditprofile);
    rightLayout->addWidget(rightlog_out);

    QHBoxLayout* mainLayout = new QHBoxLayout(twoPlayersWidget);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(middle);
    mainLayout->addLayout(rightLayout);
    mainLayout->setStretch(0, 2);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 2);
}
