
#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingsWidget(this)
{
    ui->setupUi(this); 
    QWidget *centralWidget = new QWidget(this);
    users = new usersWidget(this);
    //openLoginButton = new QPushButton("Abrir LoginPage",this);
    //connect(openLoginButton, &QPushButton::clicked, this, &MainWindow::openLoginPage);
    // Crear un layout vertical
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    QVBoxLayout *layoutV = new QVBoxLayout(centralWidget);
    QVBoxLayout *layoutVLeft = new QVBoxLayout(centralWidget);
    QHBoxLayout *layoutH = new QHBoxLayout(centralWidget);


    QPushButton *settingsButton = new QPushButton("Abrir Ajustes", this);
    connect(settingsButton, &QPushButton::clicked, [this]() {
        settingsWidget.setVisible(!settingsWidget.isVisible());
        settingsWidget.setGeometry(this->pos().x() + width()*0.1, height()*0.75, 200, 200);
    });
    settingsWidget.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    layoutVLeft->addStretch(2);
    //layoutVLeft->addWidget(openLoginButton);
    layoutVLeft->addWidget(settingsButton, 1, Qt::AlignLeft);

    layoutH->addStretch(1);
    layoutH->addWidget(users, 4);
    layoutH->addStretch(1);

    layoutV->addLayout(layoutH, 1);
    layoutV->addWidget(&board,4);

    layout->addLayout(layoutVLeft, 1);
    layout->addLayout(layoutV, 3);
    layout->addWidget(&rank, 1);

    // Configurar el tablero en el centro
    centralWidget->setLayout(layout);
    // Configurar el botón de ajustes en la parte inferior izquierda


    this->setCentralWidget(centralWidget);
    //añadir al robot (no es un usuario como tal pero vamos a contabilizarlo de esta manera para poder trabajar la base de datos mas facil)
    // Usar la biblioteca Connec4Lib
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); // Llamar al evento base

    // Recalcular la posición del widget flotante al cambiar el tamaño de la ventana
    settingsWidget.move(this->pos().x() + width()*0.1, height()*0.75);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::openLoginPage()
{
    LoginPage loginDialog(nullptr,players_playing);
    connect(&loginDialog, &LoginPage::Login_succesful, this, &MainWindow::handleLoginSuccess);
    connect(&loginDialog, &LoginPage::requestRegisterPage, this, &MainWindow::openRegisterPage);
    connect(&loginDialog,&LoginPage::requestForgotPasswordPage,this,&MainWindow::openForgotPasswordPage);
    loginDialog.exec();
}


void MainWindow::openRegisterPage(){
    RegisterPage RegisterDialog;
    //conectada a la misma funcion ya que al final para el mainwindows sigue siendo que alguien sea loggeado
    //sea por register o por loggin
    connect(&RegisterDialog, &RegisterPage::Register_succesful, this, &MainWindow::handleLoginSuccess);
    RegisterDialog.exec();
}

void MainWindow::handleLoginSuccess(Player *player){
    qDebug() << "Register was successful!" <<player->getNickName()<<"  "<<player->getPassword();
}
void MainWindow::openForgotPasswordPage(){
    ForgotPasswordPage ForgotPasswordDialog(nullptr,players_playing);
    connect(&ForgotPasswordDialog,&ForgotPasswordPage::Login_succesful,this,&MainWindow::handleLoginSuccess);
    ForgotPasswordDialog.exec();
}
*/
