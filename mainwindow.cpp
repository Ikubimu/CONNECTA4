#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingsWidget(this)
{
    ui->setupUi(this);

    // Crear el widget central
    QWidget *centralWidget = new QWidget(this);

    // Crear layouts principales
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
    layoutVLeft->addWidget(settingsButton, 1, Qt::AlignLeft);

    layoutH->addStretch(1);
    layoutH->addWidget(&userL, 1);
    layoutH->addWidget(&userR, 1);
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
