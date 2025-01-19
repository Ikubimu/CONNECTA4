#include "mainwindow.h"
#include "gameboard.h"

#include "lib/connect4.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Connect4");

    // Usar la biblioteca Connec4Lib
    Connect4& game = Connect4::getInstance();


    w.show();
    return a.exec();
}
