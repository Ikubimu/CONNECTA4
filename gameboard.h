#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class GameBoard;
}

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    ~GameBoard();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::GameBoard *ui;
    int rows;
    int cols;
    int cellSize;
    int currentPlayer;

    QVector<QVector<int>> grid;

    bool dropDisc(int column, int &row);
    bool checkWin(int row, int col);

    // Variables para la animación
    bool isAnimating;
    int animColumn;
    int animTargetRow;
    double animY; // Posición Y actual de la ficha en la animación
    int animPlayer; // Jugador que está animando
    QTimer animationTimer;
    void startAnimation(int column, int player);
};

#endif // GAMEBOARD_H
