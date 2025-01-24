#include "userswidget.h"
#define CIRCLE 100
usersWidget::usersWidget(QWidget *parent): QWidget(parent),actual(No_players){
    players_playing[0] = nullptr;
    Connect4& game = Connect4::getInstance();
    robot = game.registerPlayer("CPU", "robot@robot.com", "Password123!", QDate(1990, 1, 1), 0, QImage(":/fotos_avatar/robot.png"));
    players_playing[1] = robot;




    mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);
    updateWidget(No_players);

    QColor red(255, 0, 0); // Crear un color rojo
    this->setPlayerColors(red); // Establecer el color del jugador 1 en rojo

}

void usersWidget::updateWidget(actual_situation x){
    if(x != current)
    {
        actual = x;
    }
    // Limpiar el layout principal
    QLayoutItem *child;
    while ((child = mainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
    switch ((int)actual) {
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
QPixmap usersWidget::createCircularPixmap(const QImage &image) {
    // Tamaño del círculo
    int size = CIRCLE; // Usamos el tamaño definido por CIRCLE

    // Crear un QPixmap del tamaño del círculo
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent); // Fondo transparente

    // Crear un QPainter para dibujar en el QPixmap
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Crear un camino circular
    QPainterPath path;
    path.addEllipse(0, 0, size, size); // Dibujar un círculo del tamaño del QPixmap

    // Establecer el camino como región de recorte
    painter.setClipPath(path);

    // Escalar la imagen para que se ajuste al círculo sin deformarse
    QImage scaledImage = image.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // Calcular la posición para centrar la imagen recortada
    int x = (scaledImage.width() - size) / 2;
    int y = (scaledImage.height() - size) / 2;

    // Dibujar la imagen recortada y escalada en el QPixmap
    painter.drawImage(0, 0, scaledImage, x, y, size, size);

    return pixmap;
}

void usersWidget::openEditProfilePage(Player* player){
    EditProfilePage profileDialog(nullptr,player);
    connect(&profileDialog,&EditProfilePage::Edit_Profile_succesful,this,&usersWidget::handleEditProfilePage);
    profileDialog.exec();
}
void usersWidget::handleEditProfilePage(){
    updateWidget(actual);
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

        emit emit_current_players(1);
    }else{ // actual situation deberia de estar en dos players
        if(players_playing[0]->getNickName() == player->getNickName()){
            QMessageBox::warning(this, Labels::error, Labels::currently_logged, QMessageBox::Ok);
            return;
        }

        actual = actual_situation::Two_players;
        players_playing[1] = player;
        updateWidget(actual);

        emit emit_current_players(2);
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
        openEditProfilePage(players_playing[0]);
    } else if (buttonSender == rightEditprofile){
        openEditProfilePage(players_playing[1]);
    }
}
void usersWidget::log_out() {
    QMessageBox confirmDialog(this);
    confirmDialog.setWindowTitle(Labels::confirm_log_out);
    confirmDialog.setText(Labels::confirm_log_out_sure);
    confirmDialog.setIcon(QMessageBox::Question);
    confirmDialog.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    confirmDialog.setDefaultButton(QMessageBox::No); // Por defecto, selecciona "No"

    int ret = confirmDialog.exec(); // captura la respuesta del usuario

    if (ret == QMessageBox::No) return;

    // Si el usuario confirma, proceder con la desconexión
    QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
    if (buttonSender == leftlog_out) {
        if (actual == actual_situation::Two_players) {
            // Si hay dos jugadores, mover el jugador derecho al izquierdo
            players_playing[0] = players_playing[1];
            players_playing[1] = robot; // Restablecer el jugador derecho al robot
            actual = actual_situation::One_player;
        } else {
            // Si solo hay un jugador, desconectarlo
            players_playing[0] = nullptr;
            actual = actual_situation::No_players;
        }
    } else if (buttonSender == rightlog_out) {
        // Desconectar al jugador derecho y restablecer al robot
        players_playing[1] = robot;
        actual = actual_situation::One_player;
    }

    // Actualizar el widget para reflejar los cambios
    updateWidget(actual);

    // Emitir la señal para notificar el cambio en el número de jugadores
    if (actual == actual_situation::No_players) {
        emit emit_current_players(0);
    } else if (actual == actual_situation::One_player) {
        emit emit_current_players(1);
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

    leftButtonLogin = new QPushButton(Labels::login, this);
    connect(leftButtonLogin, &QPushButton::clicked, this, &usersWidget::openLoginPage);
    leftLayout->addWidget(leftButtonLogin);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    QLabel *rightAvatarLabel = new QLabel();
    QImage AvatarRight = QImage(":/fotos_avatar/robot.png");
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightAvatarLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightAvatarLabel);

    QLabel *rightTextLabel = new QLabel("CPU", this);
    rightTextLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightTextLabel);

    //crear el middle widget
    QWidget *middleContainer = new QWidget(this);
    whoStarts = new QComboBox(this);
    whoStarts ->addItem(Labels::player_1);
    whoStarts ->addItem(Labels::player_2);
    whoStarts ->addItem(Labels::random);
    whoStarts->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    whoStarts->setEnabled(false);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    middleLayout->addWidget(new QLabel("    VS   ", this));
    middleLayout->addWidget(new QLabel(Labels::starts,this));
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

    // Layout y avatar del jugador izquierdo
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftAvatarLabel = new QLabel(); // Usar el miembro de la clase
    leftAvatarLabel->setFixedSize(CIRCLE + 20, CIRCLE + 20); // Aumentar el tamaño para el borde
    QImage AvatarLeft = players_playing[0]->getAvatar();
    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    leftAvatarLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftAvatarLabel);

    // Nickname del jugador izquierdo
    QString nick = players_playing[0]->getNickName();
    leftLayout->addWidget(new QLabel(nick, this));

    // Botón para editar perfil del jugador izquierdo
    leftEditprofile = new QPushButton(Labels::edit_profile, this);
    connect(leftEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    leftLayout->addWidget(leftEditprofile);

    // Botón para cerrar sesión del jugador izquierdo
    leftlog_out = new QPushButton(Labels::log_out, this);
    connect(leftlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    leftLayout->addWidget(leftlog_out);

    // Layout y avatar del jugador derecho (CPU)
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightAvatarLabel = new QLabel(); // Usar el miembro de la clase
    rightAvatarLabel->setFixedSize(CIRCLE + 20, CIRCLE + 20); // Aumentar el tamaño para el borde
    QImage AvatarRight = robot->getAvatar(); // Usar la foto del robot
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightAvatarLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightAvatarLabel);

    // Etiqueta de texto para el jugador derecho (CPU)
    QLabel *rightTextLabel = new QLabel("CPU", this);
    rightTextLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightTextLabel);

    // Botón para jugar con un amigo
    rightButtonLogin = new QPushButton(Labels::play_with_friend, this);
    connect(rightButtonLogin, &QPushButton::clicked, this, &usersWidget::openLoginPage);
    rightLayout->addWidget(rightButtonLogin);

    // Crear el widget del medio (VS y selección de quién comienza)
    QWidget *middleContainer = new QWidget(this);
    whoStarts = new QComboBox(this);
    whoStarts->addItem(Labels::player_1);
    whoStarts->addItem(Labels::player_2);
    whoStarts->addItem(Labels::random);
    whoStarts->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    whoStarts->setEnabled(false);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    middleLayout->addWidget(new QLabel("    VS   ", this));
    middleLayout->addWidget(new QLabel(Labels::starts, this));
    middleLayout->addWidget(whoStarts);
    middleContainer->setLayout(middleLayout);
    middleContainer->setFixedWidth(100);

    // Layout principal del widget
    QHBoxLayout *mainLayoutWidget = new QHBoxLayout(onePlayerWidget);
    mainLayoutWidget->addLayout(leftLayout);
    mainLayoutWidget->addWidget(middleContainer);
    mainLayoutWidget->addLayout(rightLayout);

    onePlayerWidget->setLayout(mainLayoutWidget);
    mainLayout->addWidget(onePlayerWidget);

    // Resaltar al jugador activo (por defecto, el jugador 1)
    highlightPlayer(0);
}

void usersWidget::setupTwoPlayersWidget() {
    QWidget *twoPlayersWidget = new QWidget(this);

    // Layout y avatar del jugador izquierdo
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftAvatarLabel = new QLabel(); // Usar el miembro de la clase
    leftAvatarLabel->setFixedSize(CIRCLE + 20, CIRCLE + 20); // Aumentar el tamaño para el borde
    QImage AvatarLeft = players_playing[0]->getAvatar();
    leftAvatarLabel->setPixmap(createCircularPixmap(AvatarLeft));
    leftAvatarLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(leftAvatarLabel);

    // Nickname del jugador izquierdo
    QString nick = players_playing[0]->getNickName();
    leftLayout->addWidget(new QLabel(nick, this));

    // Botón para editar perfil del jugador izquierdo
    leftEditprofile = new QPushButton(Labels::edit_profile, this);
    connect(leftEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    leftLayout->addWidget(leftEditprofile);

    // Botón para cerrar sesión del jugador izquierdo
    leftlog_out = new QPushButton(Labels::log_out, this);
    connect(leftlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    leftLayout->addWidget(leftlog_out);

    // Layout y avatar del jugador derecho
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightAvatarLabel = new QLabel(); // Usar el miembro de la clase
    rightAvatarLabel->setFixedSize(CIRCLE + 20, CIRCLE + 20); // Aumentar el tamaño para el borde
    QImage AvatarRight = players_playing[1]->getAvatar(); // Usar la foto del segundo jugador
    rightAvatarLabel->setPixmap(createCircularPixmap(AvatarRight));
    rightAvatarLabel->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(rightAvatarLabel);

    // Nickname del jugador derecho
    QString nick2 = players_playing[1]->getNickName();
    rightLayout->addWidget(new QLabel(nick2, this));

    // Botón para editar perfil del jugador derecho
    rightEditprofile = new QPushButton(Labels::edit_profile, this);
    connect(rightEditprofile, &QPushButton::clicked, this, &usersWidget::openConfigureProfile);
    rightLayout->addWidget(rightEditprofile);

    // Botón para cerrar sesión del jugador derecho
    rightlog_out = new QPushButton(Labels::log_out, this);
    connect(rightlog_out, &QPushButton::clicked, this, &usersWidget::log_out);
    rightLayout->addWidget(rightlog_out);

    // Crear el widget del medio (VS y selección de quién comienza)
    QWidget *middleContainer = new QWidget(this);
    whoStarts = new QComboBox(this);
    whoStarts->addItem(Labels::player_1);
    whoStarts->addItem(Labels::player_2);
    whoStarts->addItem(Labels::random);
    whoStarts->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    middleLayout->addWidget(new QLabel("    VS   ", this));
    middleLayout->addWidget(new QLabel(Labels::who_starts, this));
    middleLayout->addWidget(whoStarts);
    middleContainer->setLayout(middleLayout);
    middleContainer->setFixedWidth(120);

    // Layout principal del widget
    QHBoxLayout *mainLayoutWidget = new QHBoxLayout(twoPlayersWidget);
    mainLayoutWidget->addLayout(leftLayout);
    mainLayoutWidget->addWidget(middleContainer);
    mainLayoutWidget->addLayout(rightLayout);

    twoPlayersWidget->setLayout(mainLayoutWidget);
    mainLayout->addWidget(twoPlayersWidget);

    // Resaltar al jugador activo (por defecto, el jugador 1)
    highlightPlayer(0);
}

static QColor get_opposite_color(QColor color)
{
    int r = 255 - color.red();
    int g = 255 - color.green();
    int b = 255 - color.blue();

    return QColor(r, g, b);
}

void usersWidget::setPlayerColors(const QColor &color1) {
    p1Color = color1;
    p2Color = get_opposite_color(p1Color);

    updateWidget(actual);
}

void usersWidget::drawPlayerBorder(QLabel *label, const QColor &borderColor) {
    if (!label) return;

    // Obtener el QPixmap actual del QLabel
    QPixmap originalPixmap = label->pixmap(Qt::ReturnByValue);
    if (originalPixmap.isNull()) return;

    // Crear un nuevo QPixmap con un tamaño ligeramente mayor para el marco
    int borderWidth = 6; // Ancho del borde
    QPixmap borderedPixmap(originalPixmap.size() + QSize(borderWidth * 2, borderWidth * 2));
    borderedPixmap.fill(Qt::transparent); // Fondo transparente

    // Crear un QPainter para dibujar en el nuevo QPixmap
    QPainter painter(&borderedPixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Dibujar el marco con el color especificado
    painter.setPen(QPen(borderColor, borderWidth)); // Marco con el color proporcionado
    painter.setBrush(Qt::NoBrush); // Sin relleno
    painter.drawEllipse(borderedPixmap.rect().adjusted(borderWidth / 2, borderWidth / 2, -borderWidth / 2, -borderWidth / 2));

    // Dibujar la imagen original centrada dentro del marco
    painter.drawPixmap(borderWidth, borderWidth, originalPixmap);

    // Establecer el nuevo QPixmap en el QLabel
    label->setPixmap(borderedPixmap);
}


void usersWidget::highlightPlayer(int playerIndex) {
    // Quitar el borde de ambos jugadores primero
    if (leftAvatarLabel) {
        leftAvatarLabel->setPixmap(createCircularPixmap(players_playing[0]->getAvatar())); // Restaurar la imagen original
    }
    if (rightAvatarLabel) {
        rightAvatarLabel->setPixmap(createCircularPixmap(players_playing[1]->getAvatar())); // Restaurar la imagen original
    }

    // Aplicar el borde al jugador activo con el color correspondiente
    if (playerIndex == 0 && leftAvatarLabel) {
        drawPlayerBorder(leftAvatarLabel, p1Color); // Dibujar el borde en el jugador izquierdo con el color de p1
    } else if (playerIndex == 1 && rightAvatarLabel) {
        drawPlayerBorder(rightAvatarLabel, p2Color); // Dibujar el borde en el jugador derecho con el color de p2
    }
}
