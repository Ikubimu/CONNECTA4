#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);

    // Crear un layout vertical
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    QVBoxLayout *layoutV = new QVBoxLayout(centralWidget);
    QHBoxLayout *layoutH = new QHBoxLayout(centralWidget);

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

}

MainWindow::~MainWindow()
{
    delete ui;
}
