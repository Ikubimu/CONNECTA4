#include "gameboard.h"
#include "ui_gameboard.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QMessageBox>

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameBoard)
    , rows(6)
    , cols(7)
    , currentPlayer(1)
{
    ui->setupUi(this);
    grid.resize(rows, QVector<int>(cols, 0));
    setMinimumSize(cols * 40, rows * 40);
}

GameBoard::~GameBoard()
{
    delete ui;
}

void GameBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // a las posiciones (x,y) de cualquier punto se le suman la posicion (x0,y0) donde se pinta el centralQWidget sobre MainWindow
    QRect espacioJuego = geometry();

    // calcula el tamaño de la celda
    if (espacioJuego.width() / cols < espacioJuego.height() / rows) {
        cellSize = espacioJuego.width() / cols;
    } else {
        cellSize = espacioJuego.height() / rows;
    }
    int x0 = (espacioJuego.width() - (cellSize * cols)) / 2;
    int y0 = (espacioJuego.height() - (cellSize * rows)) / 2;

    // Dibujar la fondo
    painter.setBrush(Qt::gray);
    painter.drawRect(x0, y0, cellSize * cols, cellSize * rows);

    // Dibujar celdas
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // Dibujar círculo para cada celda
            //QRectF circleRect(c * cellSize + 5, r * cellSize + 5, cellSize - 10, cellSize - 10);
            QRectF circleRect(x0 + (c * cellSize + 5),
                              y0 + (r * cellSize + 5),
                              cellSize - 10,
                              cellSize - 10);
            if (grid[r][c] == 0) {
                painter.setBrush(Qt::white);
            } else if (grid[r][c] == 1) {
                painter.setBrush(Qt::red);
            } else {
                painter.setBrush(Qt::yellow);
            }
            painter.drawEllipse(circleRect);
        }
    }

}

void GameBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        QRect geom = geometry();
        int with = geom.width();
        int x0 = (with-(cols * cellSize))/2;
        // hay que restar el x0
        int column = (x-x0) / cellSize;
        if(column >= 0 && column < cols){
            int row;
            if(dropDisc(column, row)){

                update();

                //Verificar si el movimiento actual gana el juego
                if(checkWin(row, column)){

                    QMessageBox::information(this, "Victoria", QString("¡Jugador %1 ha ganado!").arg(currentPlayer));

                    return;
                }

                // Cambiar de jugador
                currentPlayer = (currentPlayer == 1) ? 2 : 1;

            }
            else{
                QMessageBox::warning(this, "Columna Llena", "La columna seleccionada está llena. Por favor, elige otra.");
            }
        }

    }
}

bool GameBoard::dropDisc(int column, int &row)
{
    for (int r = rows - 1; r >= 0; --r) {
        if (grid[r][column] == 0) {
            grid[r][column] = currentPlayer;
            row = r;
            return true;
        }
    }
    return false; // Columna llena
}
bool GameBoard::checkWin(int row, int col)
{
    int player = grid[row][col];
    if(player == 0)
        return false;

    // Direcciones: horizontal, vertical, diagonal /, diagonal \*
    const QVector<QPair<int, int>> directions
        = {
            {0, 1}, {1, 0}, {1, 1}, {1, -1}
        };

    for(auto &dir : directions){
        int count = 1;

        // Dirección positiva
        int r = row + dir.first;
        int c = col + dir.second;
        while(r >=0 && r < rows && c >=0 && c < cols && grid[r][c] == player){
            count++;
            r += dir.first;
            c += dir.second;
        }

        // Dirección negativa
        r = row - dir.first;
        c = col - dir.second;
        while(r >=0 && r < rows && c >=0 && c < cols && grid[r][c] == player){
            count++;
            r -= dir.first;
            c -= dir.second;
        }

        if(count >=4)
            return true;
    }

    return false;
}
