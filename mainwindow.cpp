#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Crear el widget central
    QWidget *centralWidget = new QWidget(this);

    // Crear layouts principales
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    // Configurar el layout superior
    topLayout->addStretch(1);
    topLayout->addWidget(&userL, 1);
    topLayout->addWidget(&userR, 1);
    topLayout->addStretch(1);

    // Configurar el tablero en el centro
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addWidget(&board, 4);

    // Configurar el botón de ajustes en la parte inferior izquierda
    QPushButton *settingsButton = new QPushButton("Abrir Ajustes", this);
    connect(settingsButton, &QPushButton::clicked, [this]() {
        settingsWidget.setVisible(!settingsWidget.isVisible());
    });

    bottomLayout->addWidget(settingsButton, 0, Qt::AlignLeft);
    bottomLayout->addStretch(1);

    mainLayout->addLayout(bottomLayout, 0);

    // Configurar el widget de ajustes flotante
    settingsWidget.setParent(this); // Configurar el padre como la ventana principal
    settingsWidget.setVisible(false);
    settingsWidget.setGeometry(20, height() - 200, 250, 150); // Posición inicial en la esquina inferior izquierda

    // Establecer un color de fondo sólido para el widget de ajustes
    QPalette palette = settingsWidget.palette();
    palette.setColor(QPalette::Window, Qt::black); // Usar QPalette::Window en lugar de QPalette::Background
    settingsWidget.setAutoFillBackground(true);
    settingsWidget.setPalette(palette);



    // Establecer el widget central
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); // Llamar al evento base

    // Recalcular la posición del widget flotante al cambiar el tamaño de la ventana
    settingsWidget.move(40, height() - settingsWidget.height() - 50);
}

MainWindow::~MainWindow()
{
    delete ui;
}
