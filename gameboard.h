#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTimer>
#include "language.h"

namespace Ui {
class GameBoard;
}

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    ~GameBoard();

    enum results
    {
        draw = 0,
        win,
        lost
    };

    void setColorPieces(QColor color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::GameBoard *ui;
    int rows;
    int cols;
    int cellSize;
    int currentPlayer;
    int column_selected = 0;

    QVector<QVector<int>> grid;

    bool dropDisc(int column, int &row);
    bool canDropDisc(int column, int &row);
    bool checkWin(int row, int col);
    bool checkFullGrid();
    void resetBoard();

    // Variables para la animación
    bool isAnimating;
    int animColumn;
    int animTargetRow;
    double animY; // Posición Y actual de la ficha en la animación
    int animPlayer; // Jugador que está animando
    QTimer animationTimer;
    void startAnimation(int column, int player);

    bool cpu_on{true};

    enum states{
        disable=0,
        player1,
        player2
    };

    states curr_state{disable};

    QColor p1, p2;

public slots:
    void receive_current_players(int num);

signals:
    void emit_result(results data);
};

#endif // GAMEBOARD_H
