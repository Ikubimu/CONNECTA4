#include "mainwindow.h"
#include "gameboard.h"

#include "lib/connect4.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameBoard w;
    w.setWindowTitle("Connect4");

    // Usar la biblioteca Connec4Lib
    Connect4& game = Connect4::getInstance();


    Player* player = game.registerPlayer("Player1", "player1@example.com", "Password123!", QDate(1990, 1, 1), 100);
    if (player) {
        qDebug() << "Jugador registrado:" << player->getNickName();
    }
    w.show();
    return a.exec();
}
