#include "mainwindow.h"
#include "gameboard.h"

#include "lib/connect4.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Connect4");
    w.show();
    return a.exec();
}
