#include "gameboard.h"
#include "ui_gameboard.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QMessageBox>
#include <climits>

static int elegirJugada(QVector<QVector<int>>& tablero, int profundidad);
static QColor get_opposite_color(QColor color);

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameBoard)
    , rows(6)
    , cols(7)
    , currentPlayer(1)
    , p1(255,0,0)
    , p2(0, 255, 255)
{
    ui->setupUi(this);
    grid.resize(rows, QVector<int>(cols, 0));
    setMinimumSize(cols * 40, rows * 40);
    setMouseTracking(true);
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
                painter.setBrush(p1);
            } else {
                painter.setBrush(p2);
            }
            painter.drawEllipse(circleRect);
        }
    }


    //Pintado de columna donde apunta el ratón Para el uso de estos efectos se ha usado ChatGPT
    // Crear el gradiente radial
    QRadialGradient gradient(x0 + column_selected * cellSize + cellSize / 2,
                             y0 + espacioJuego.height() * 0.9,
                             300);

    // Configurar colores del gradiente
    QColor color, color64, color128;
    if(currentPlayer == 1)
    {
        color = p1;
    }
    else
    {
        color = p2;

    }
    color128 = color;
    color64 = color;
    color128.setAlpha(128);
    color64.setAlpha(64);
    gradient.setColorAt(1.0, color128); // Rojo semitransparente (menor opacidad)
    gradient.setColorAt(0.5, color64); // Rojo más tenue a mitad de camino
    gradient.setColorAt(0.2, Qt::transparent);

    // Configurar el pintor
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);

    // Dibujar el rectángulo con el gradiente aplicado
    painter.drawRect(x0 + column_selected * cellSize, y0, cellSize, espacioJuego.height());

}


void GameBoard::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    QRect geom = geometry();
    int with = geom.width();
    int x0 = (with-(cols * cellSize))/2;
    // hay que restar el x0
    column_selected = (x-x0) / cellSize;
    repaint();
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
                    emit emit_result((results)currentPlayer);
                    resetBoard();
                    return;
                }
                else if(checkFullGrid()){

                    QMessageBox::information(this, "Empate", QString("No queda espacio de juego"));
                    emit emit_result((results)currentPlayer);
                    resetBoard();
                    return;
                }

                // Cambiar de jugador
                currentPlayer = (currentPlayer == 1) ? 2 : 1;

                if(currentPlayer==2 && cpu_on)
                {
                    int columnaCPU = elegirJugada(grid, 5);
                    dropDisc(columnaCPU, row);
                    if(checkWin(row, columnaCPU)){

                        QMessageBox::information(this, "Victoria", QString("¡Jugador %1 ha ganado!").arg(currentPlayer));
                        emit emit_result((results)currentPlayer);
                        resetBoard();
                        return;
                    }
                    else if(checkFullGrid()){

                        QMessageBox::information(this, "Empate", QString("No queda espacio de juego"));
                        emit emit_result((results)currentPlayer);
                        resetBoard();
                        return;
                    }


                    currentPlayer = 1;

                }

            }
            else{
                QMessageBox::warning(this, Labels::empty_column, Labels::empty_column_phrase);
            }
        }

    }
}

bool GameBoard::checkFullGrid()
{
    for(int i=0; i<grid.size(); i++)
    {
        if(grid[i].last() == 0)
        {
            return false;
        }
    }
    return true;
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

void GameBoard::resetBoard()
{
    for(int i = 0; i<grid.size(); i++)
    {
        for(int j = 0; j<grid[i].size(); j++)
        {
            grid[i][j] = 0;
        }
    }
    currentPlayer = 1;
}

void GameBoard::setColorPieces(QColor color)
{
    p1 = color;
    p2 = get_opposite_color(color);
}


static QColor get_opposite_color(QColor color)
{
    int r = 255 - color.red();
    int g = 255 - color.green();
    int b = 255 - color.blue();

    return QColor(r, g, b);
}





/*
 * El siguiente código utiliza un sistema de heurística para jugar contra un bot
 * El código se ha cogido de fuente externa cómo lo es ChatGPT debido a que
 * ningún objetivo de este trabajo contemplaba hacer esta funcionalidad de esta manera
 */



static int evaluarHeuristica(const QVector<QVector<int>>& tablero, int jugador) {
    int puntaje = 0;
    int FILAS = tablero.size();
    int COLUMNAS = tablero[0].size();

    auto contarConsecutivos = [&](int fila, int columna, int deltaFila, int deltaColumna, int jugador) {
        int cuenta = 0;
        int espaciosVacios = 0;
        for (int i = 0; i < 4; i++) {
            int nuevaFila = fila + i * deltaFila;
            int nuevaColumna = columna + i * deltaColumna;
            if (nuevaFila >= 0 && nuevaFila < FILAS && nuevaColumna >= 0 && nuevaColumna < COLUMNAS) {
                if (tablero[nuevaFila][nuevaColumna] == jugador) {
                    cuenta++;
                } else if (tablero[nuevaFila][nuevaColumna] == 0) {
                    espaciosVacios++;
                }
            }
        }
        return (espaciosVacios + cuenta == 4) ? cuenta : 0; // Valorar solo si tiene potencial.
    };

    for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            if (tablero[fila][columna] == jugador) {
                // Horizontal
                int cuenta = contarConsecutivos(fila, columna, 0, 1, jugador);
                puntaje += (cuenta == 3) ? 1000 : (cuenta == 2) ? 50 : 0;

                // Vertical
                cuenta = contarConsecutivos(fila, columna, 1, 0, jugador);
                puntaje += (cuenta == 3) ? 1000 : (cuenta == 2) ? 50 : 0;

                // Diagonal descendente (\)
                cuenta = contarConsecutivos(fila, columna, 1, 1, jugador);
                puntaje += (cuenta == 3) ? 1000 : (cuenta == 2) ? 50 : 0;

                // Diagonal ascendente (/)
                cuenta = contarConsecutivos(fila, columna, -1, 1, jugador);
                puntaje += (cuenta == 3) ? 1000 : (cuenta == 2) ? 50 : 0;
            }
        }
    }

    // Valorar la posición central (estrategia).
    for (int fila = 0; fila < FILAS; fila++) {
        puntaje += (tablero[fila][COLUMNAS / 2] == jugador) ? 10 : 0;
    }

    return puntaje;
}

static int evaluarTablero(const QVector<QVector<int>>& tablero) {
    int puntuacionCPU = evaluarHeuristica(tablero, 2);
    int puntuacionJugador = evaluarHeuristica(tablero, 1);
    return puntuacionCPU - puntuacionJugador;
}

static QVector<int> obtenerMovimientosDisponibles(const QVector<QVector<int>>& tablero) {
    int FILAS = tablero.size();
    int COLUMNAS = tablero[0].size();
    QVector<int> movimientos;
    for (int j = 0; j < COLUMNAS; j++) {
        if (tablero[0][j] == 0) {
            movimientos.push_back(j);
        }
    }
    return movimientos;
}

static int minimax(QVector<QVector<int>>& tablero, int profundidad, int alpha, int beta, bool esMaximizador) {
    int FILAS = tablero.size();
    int COLUMNAS = tablero[0].size();
    QVector<int> movimientos = obtenerMovimientosDisponibles(tablero);

    // Evaluación de condiciones terminales.
    int puntuacion = evaluarTablero(tablero);
    if (profundidad == 0 || movimientos.isEmpty()) {
        return puntuacion;
    }

    if (esMaximizador) {
        int mejorValor = INT_MIN;
        for (int col : movimientos) {
            for (int fila = FILAS - 1; fila >= 0; fila--) {
                if (tablero[fila][col] == 0) {
                    tablero[fila][col] = 2;
                    break;
                }
            }
            mejorValor = std::max(mejorValor, minimax(tablero, profundidad - 1, alpha, beta, false));
            alpha = std::max(alpha, mejorValor);
            for (int fila = 0; fila < FILAS; fila++) {
                if (tablero[fila][col] == 2) {
                    tablero[fila][col] = 0;
                    break;
                }
            }
            if (beta <= alpha) break;
        }
        return mejorValor;
    } else {
        int mejorValor = INT_MAX;
        for (int col : movimientos) {
            for (int fila = FILAS - 1; fila >= 0; fila--) {
                if (tablero[fila][col] == 0) {
                    tablero[fila][col] = 1;
                    break;
                }
            }
            mejorValor = std::min(mejorValor, minimax(tablero, profundidad - 1, alpha, beta, true));
            beta = std::min(beta, mejorValor);
            for (int fila = 0; fila < FILAS; fila++) {
                if (tablero[fila][col] == 1) {
                    tablero[fila][col] = 0;
                    break;
                }
            }
            if (beta <= alpha) break;
        }
        return mejorValor;
    }
}

static int elegirJugada(QVector<QVector<int>>& tablero, int profundidad) {
    QVector<int> movimientos = obtenerMovimientosDisponibles(tablero);
    int mejorJugada = -1;
    int mejorValor = INT_MIN;

    int FILAS = tablero.size();

    for (int col : movimientos) {
        for (int fila = FILAS - 1; fila >= 0; fila--) {
            if (tablero[fila][col] == 0) {
                tablero[fila][col] = 2;
                break;
            }
        }
        int valor = minimax(tablero, profundidad - 1, INT_MIN, INT_MAX, false);
        if (valor > mejorValor) {
            mejorValor = valor;
            mejorJugada = col;
        }
        for (int fila = 0; fila < FILAS; fila++) {
            if (tablero[fila][col] == 2) {
                tablero[fila][col] = 0;
                break;
            }
        }
    }

    return mejorJugada;
}
