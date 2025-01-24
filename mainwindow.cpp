#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lib/connect4.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingsWidget(this)
    , board(this)
{
    ui->setupUi(this);


    connect(&settingsWidget, &SettingsUser::languagechoosed,this,&MainWindow::change_language_signal);
    connect(&settingsWidget, &SettingsUser::styleMode,this,&MainWindow::changeAllStyles);


    QWidget *centralWidget = new QWidget(this);
    users = new usersWidget(this);

    // Crear un layout vertical
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    QVBoxLayout *layoutV = new QVBoxLayout(centralWidget);
    QVBoxLayout *layoutVLeft = new QVBoxLayout(centralWidget);
    QHBoxLayout *layoutH = new QHBoxLayout(centralWidget);

    // Configurar el botón de cambio de color
    QIcon colorIcon(":/fotos_varias/changecolor.png"); // Ruta al ícono
    colorPickerButton = new QPushButton(this);
    colorPickerButton->setIcon(colorIcon); // Asignar el ícono al botón
    colorPickerButton->setIconSize(QSize(60, 60)); // Ajustar el tamaño del ícono
    colorPickerButton->setStyleSheet("QPushButton { border: none; background: transparent; }"); // Estilo para eliminar el borde y el fondo
    colorPickerButton->setText(""); // Eliminar el texto (opcional)



    // Conectar el botón con el cambio de color
    connect(colorPickerButton, &QPushButton::clicked, this, &MainWindow::onChangePieceColor);

    historyButton = new QPushButton(Labels::history, this);
    connect(historyButton, &QPushButton::clicked, [&]() {
        if (hist == nullptr) {
            hist = new gameHistory(nullptr);
            hist->setStyleSheet(this->styleSheet());
            hist->setAttribute(Qt::WA_DeleteOnClose);
            hist->setWindowTitle(Labels::history);
            hist->resize(400, 300); // Ajusta el tamaño de la ventana según sea necesario
            connect(hist, &QObject::destroyed, [&]() {
                hist = nullptr;
            });
            hist->show();
        }
    });

    QIcon icon(":/fotos_varias/settings.png");
    settingsButton = new QPushButton("", this);
    settingsButton->setIcon(icon);
    settingsButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
    settingsButton->setIconSize(QSize(60, 60));
    connect(settingsButton, &QPushButton::clicked, [this]() {
        settingsWidget.setVisible(!settingsWidget.isVisible());
        updateSettingsWidgetPosition(); // Recalcular la posición cuando se muestra
    });
    settingsWidget.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    layoutVLeft->addStretch(1);
    layoutVLeft->addWidget(colorPickerButton);
    layoutVLeft->addWidget(historyButton);
    layoutVLeft->addWidget(settingsButton, 1, Qt::AlignLeft);

    layoutH->addStretch(1);
    layoutH->addWidget(users, 4);
    layoutH->addStretch(1);

    layoutV->addLayout(layoutH, 1);
    layoutV->addWidget(&board, 4);

    layout->addLayout(layoutVLeft, 1);
    layout->addLayout(layoutV, 3);
    layout->addWidget(&rank, 2);

    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
    //Poner por defecto el lenguaje en castellano

    //lamada para que las funciones cojan los estilos del codigo de estilos
    QFile file(":/estilos/estilos.qss"); // Ruta al archivo en el recurso
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(file.readAll());
        this->setStyleSheet(styleSheet);
        rank.setStyleSheet(styleSheet);
        users->setStyleSheet(styleSheet);
        file.close();
    }

    // Registro de jugadores (ejemplo)
    Connect4& game = Connect4::getInstance();
    Player* machine_player = game.registerPlayer("CPU", "robot@robot.com", "Password123!", QDate(1990, 1, 1), 0);
    Player* p1 = game.registerPlayer("oscar1", "oscar@oscar.com", "Password123!", QDate(1990, 1, 1), 0);
    players_playing[0] = p1;
    players_playing[1] = machine_player;

    // Conectar mainwindows con la señal de victoria de gameboard
    connect(&board, &GameBoard::emit_result, this, &MainWindow::receive_result);
    // Conectar la señal de cambio de turno
    connect(&board, &GameBoard::turnChanged, this, &MainWindow::onTurnChanged); // <-- Aquí se conecta la señal

    connect(users, &usersWidget::emit_current_players, &board, &GameBoard::receive_current_players);

    // Conectar la señal de cambio de turno
    connect(&board, &GameBoard::turnChanged, this, &MainWindow::onTurnChanged);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // Recalcular la posición del widget flotante
    if (settingsWidget.isVisible()) {
        updateSettingsWidgetPosition();
    }
}

void MainWindow::onChangePieceColor() {
    // Abrir un selector de color
    QColor selectedColor = QColorDialog::getColor(Qt::red, this, Labels::select_color);

    // Validar si se seleccionó un color válido
    if (selectedColor.isValid()) {
        board.setColorPieces(selectedColor); // Llamar a la función del tablero
        users->setPlayerColors(selectedColor);
    }
}

void MainWindow::updateSettingsWidgetPosition() {
    // Dimensiones del widget
    int widgetWidth = 200;
    int widgetHeight = 200;

    // Obtener la posición global de la ventana principal
    QPoint globalPos = this->mapToGlobal(QPoint(0, 0));

    // Cálculo de posición
    int x = globalPos.x() + (this->width() - widgetWidth) / 7; // Centrado horizontalmente
    int y = globalPos.y() + this->height() - widgetHeight - 40; // A 20 píxeles del borde inferior

    settingsWidget.setGeometry(x, y, widgetWidth, widgetHeight);
}

MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::change_language_signal(int idioma){
    switch(idioma){
        case 0:
        Language::change_to_tortilla_patata();
        break;
    case 1:
        Language::change_to_fish_and_chips();
        break;
    case 2:
        Language::change_to_Mbappe();
        break;
    }
    historyButton->setText(Labels::history);
    users->updateWidget(actual_situation::current);
}

void MainWindow::receive_result(GameBoard::results data) {
    Player* winner;
    Player* looser;
    int players = users->get_players(players_playing);
    switch (data) {
    case GameBoard::win:
        winner = players_playing[0];
        looser = players_playing[1];
        winner->addPoints(30);
        break;
    case GameBoard::draw:
        winner = players_playing[1];
        looser = players_playing[0];
        winner->addPoints(10);
        winner->addPoints(10);
        break;
    case GameBoard::lost:
        winner = players_playing[1];
        looser = players_playing[0];
        winner->addPoints(30);
        break;
    }

    if (players_playing[1]->getNickName() != "CPU") {
        Connect4& instance = Connect4::getInstance();
        // La base de datos no contempla el caso de que sea empate, ponemos a un ganador aunque haya reparto de puntos
        instance.registerRound(QDateTime::currentDateTime(), winner, looser);
    }
}

// En MainWindow o GameBoard
void MainWindow::onTurnChanged(int currentPlayerIndex) {
    users->highlightPlayer(currentPlayerIndex); // Resaltar al jugador activo
}



void MainWindow::changeAllStyles(bool mode)
{
    if(mode)
    {
        QFile file(":/estilos/estilos_modo_noche.qss"); // Ruta al archivo en el recurso
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QString::fromUtf8(file.readAll());
            this->setStyleSheet(styleSheet);
            rank.setStyleSheet(styleSheet);
            settingsWidget.setStyleSheet(styleSheet);
            users->setStyleSheet(styleSheet);
            file.close();
        }
    }
    else
    {
        QFile file(":/estilos/estilos.qss"); // Ruta al archivo en el recurso
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QString::fromUtf8(file.readAll());
            this->setStyleSheet(styleSheet);
            rank.setStyleSheet(styleSheet);
            settingsWidget.setStyleSheet(styleSheet);
            users->setStyleSheet(styleSheet);
            file.close();
        }
    }
}
