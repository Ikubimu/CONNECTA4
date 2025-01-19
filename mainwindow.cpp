#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); 
    QWidget *centralWidget = new QWidget(this);
    openLoginButton = new QPushButton("Abrir LoginPage",this);
    connect(openLoginButton, &QPushButton::clicked, this, &MainWindow::openLoginPage);
    // Crear un layout vertical
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    QVBoxLayout *layoutV = new QVBoxLayout(centralWidget);
    QHBoxLayout *layoutH = new QHBoxLayout(centralWidget);


    layout->addWidget(openLoginButton);
    layout->addStretch(1);

    layoutH->addStretch(1);
    layoutH->addWidget(&userL, 1);
    layoutH->addWidget(&userR, 1);
    layoutH->addStretch(1);

    layoutV->addLayout(layoutH, 1);
    layoutV->addWidget(&board,4);
    layout->addLayout(layoutV, 3);
    layout->addWidget(&rank, 1);


    // Establecer el layout como el layout principal del widget central
    centralWidget->setLayout(layout);

    // Establecer el widget central
    this->setCentralWidget(centralWidget);
    //añadir al robot (no es un usuario como tal pero vamos a contabilizarlo de esta manera para poder trabajar la base de datos mas facil)
    // Usar la biblioteca Connec4Lib
    Connect4& game = Connect4::getInstance();
    Player* machine_player = game.registerPlayer("ROBOT", "robot@robot.com", "Password123!", QDate(1990, 1, 1), 0);
    Player* p1 = game.registerPlayer("oscar1", "oscar@oscar.com", "Password123!", QDate(1990, 1, 1), 0);
    players_playing[1] = machine_player;

}

MainWindow::~MainWindow()
{
    delete ui;
}

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
