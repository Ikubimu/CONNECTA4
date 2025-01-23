#include "userswidget.h"
#define SIZE_REL 0.9

usersWidget::usersWidget(QWidget *parent,Player *players[2]): QWidget(parent){
    actual = actual_situation::No_players;
    players_playing[0] = players[0];
    players_playing[1] = players[1];
    robot = players[1];
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

//Para que salga circular  la imagen  buscado en internet
    AvatarLeft = QImage(":/fotos_avatar/noUserImage.jpg");
    AvatarRight = QImage(":/fotos_avatar/robot.jpg");

    QLabel *leftAvatarLabel = new QLabel(this);
    QLabel *rightAvatarLabel = new QLabel(this);

    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));



    mainWidget = new QHBoxLayout(this);
    left_widget = new QVBoxLayout();
    right_widget = new QVBoxLayout();
    QVBoxLayout *middle = new QVBoxLayout(); // no lo añado dentro de la clase debido a que una vez creado no variara

    middle->addWidget(new QLabel(" VS ",this));
    middle->addWidget(whoStarts);

    left_widget->addWidget(leftAvatarLabel);
    left_widget->addWidget(leftButtonLogin);

    right_widget->addWidget(rightAvatarLabel);
    right_widget->addWidget(new QLabel("Robot"));
    right_widget->addWidget(Difficulty_Level_Robot);

    mainWidget->addLayout(left_widget);
    mainWidget->addLayout(middle);
    mainWidget->addLayout(right_widget);
}

void usersWidget::openLoginPage()
{
    LoginPage loginDialog(nullptr,players_playing);
    connect(&loginDialog, &LoginPage::Login_succesful, this, &usersWidget::handleLoginSuccess);
    connect(&loginDialog, &LoginPage::requestRegisterPage, this, &usersWidget::openRegisterPage);
    connect(&loginDialog,&LoginPage::requestForgotPasswordPage,this,&usersWidget::openForgotPasswordPage);
    loginDialog.exec();
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
QPixmap usersWidget::createCircularPixmap(const QImage &image)
{
    QPixmap pixmap = QPixmap::fromImage(image).scaled(100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QBitmap mask(100, 100);
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setBrush(Qt::color1);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawEllipse(0, 0, 100, 100);
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
void usersWidget::updateWidget(actual_situation x){

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
        updateWidget(One_player);
    }
}
