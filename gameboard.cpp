#include "gameboard.h"
#include "ui_gameboard.h"

#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <QMessageBox>
#include <climits>

// Prototipos de funciones auxiliares
static int elegirJugada(QVector<QVector<int>> &tablero, int profundidad);
static QColor get_opposite_color(QColor color);

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::GameBoard),
    rows(6),
    cols(7),
    currentPlayer(1),
    p1(255, 0, 0),
    p2(0, 255, 255),
    isAnimating(false)
{
    ui->setupUi(this);
    grid.resize(rows, QVector<int>(cols, 0));
    setMinimumSize(cols * 40, rows * 40);
    setMouseTracking(true);

    // En el callback del temporizador de animación
    connect(&animationTimer, &QTimer::timeout, this, [this]() {
        animY += 5; // Incremento de posición
        if (animY >= animTargetRow * cellSize) {
            // Detener animación y colocar ficha
            animationTimer.stop();
            grid[animTargetRow][animColumn] = animPlayer;
            isAnimating = false;
            update();

            // Verificar condiciones de victoria o empate
            if (checkWin(animTargetRow, animColumn)) {

                QMessageBox::information(this, Labels::victory, QString(Labels::player_win + ":  %1 ").arg(animPlayer));
                emit emit_result((results)animPlayer);
                resetBoard();
                return;
            } else if (checkFullGrid()) {
                QMessageBox::information(this, Labels::draw,Labels::draw);
                emit emit_result(draw);
                resetBoard();
                return;
            }

            // Cambiar de jugador
            currentPlayer = (currentPlayer == 1) ? 2 : 1;

            // Emitir la señal de cambio de turno

            if (!cpu_on){

                emit turnChanged(currentPlayer-1); // <-- Aquí se emite la señal

            }


            // Turno del CPU
            if (currentPlayer == 2 && cpu_on) {
                int cpuColumn = elegirJugada(grid, 5);
                int row;
                startAnimation(cpuColumn, 2);
            }
        }
        update(); // Redibujar mientras cae la ficha
    });
}

GameBoard::~GameBoard()
{
    delete ui;
}

void GameBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect espacioJuego = geometry();
    cellSize = qMin(espacioJuego.width() / cols, espacioJuego.height() / rows);
    int x0 = (espacioJuego.width() - (cellSize * cols)) / 2;
    int y0 = (espacioJuego.height() - (cellSize * rows)) / 2;

    // Dibujar el fondo
    painter.setBrush(Qt::gray);
    painter.drawRect(x0, y0, cellSize * cols, cellSize * rows);

    // Dibujar celdas
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
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

    if(curr_state == disable) return;
    // Dibujar ficha animada
    if (isAnimating) {
        QRectF animRect(x0 + (animColumn * cellSize + 5),
                        y0 + animY + 5,
                        cellSize - 10,
                        cellSize - 10);
        painter.setBrush(animPlayer == 1 ? p1 : p2);
        painter.drawEllipse(animRect);
    }

    // Indicar columna seleccionada
    QRadialGradient gradient(x0 + column_selected * cellSize + cellSize / 2,
                             y0 + cellSize / 2, 300);  // Ajustar posición del gradiente
    QColor color = (currentPlayer == 1) ? p1 : p2;
    color.setAlpha(128);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, Qt::transparent);
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(x0 + column_selected * cellSize, y0, cellSize, espacioJuego.height());
}


void GameBoard::mouseMoveEvent(QMouseEvent *event)
{
    if(curr_state==disable) return;

    int x = event->pos().x();
    QRect geom = geometry();
    int width = geom.width();
    int x0 = (width - (cols * cellSize)) / 2;

    column_selected = (x - x0) / cellSize;
    if (column_selected < 0 || column_selected >= cols) {
        column_selected = -1;  // Fuera del tablero
    }
    repaint();
}

void GameBoard::mousePressEvent(QMouseEvent *event) {
    if(curr_state==disable) return;

    if (event->button() == Qt::LeftButton && !isAnimating) {  // Evitar clics durante animación
        int x = event->pos().x();
        QRect geom = geometry();
        int width = geom.width();
        int x0 = (width - (cols * cellSize)) / 2;

        int column = (x - x0) / cellSize;
        if (column >= 0 && column < cols) {
            int row;
            if (canDropDisc(column, row)) {  // Verificar si se puede colocar la ficha
                startAnimation(column, currentPlayer); // Iniciar animación
            } else {
                QMessageBox::warning(this, Labels::empty_column, Labels::empty_column_phrase);
            }
        }
    }
}


//el codigo de la nimacion hemos usado la IA deepSeek para hacerlo, ya que creemos que queda bien pero pasaba nuestros conocimientos


void GameBoard::startAnimation(int column, int player) {
    int row;
    if (canDropDisc(column, row)) {  // Verificar sin modificar el tablero
        animColumn = column;
        animTargetRow = row;
        animY = 0;
        animPlayer = player;
        isAnimating = true;
        animationTimer.start(16);  // ~60 FPS
    } else {
        QMessageBox::warning(this, Labels::empty_column, Labels::empty_column_phrase);
    }
}

bool GameBoard::checkFullGrid()
{
    for(int i=0; i< 6; i++)
    {
        if(grid[0][i] == 0)
        {
            return false;
        }
    }
    return true;
}

bool GameBoard::dropDisc(int column, int &row) {
    for (int r = rows - 1; r >= 0; --r) {
        if (grid[r][column] == 0) {
            grid[r][column] = currentPlayer;
            row = r;
            return true;
        }
    }
    return false; // Columna llena
}

bool GameBoard::canDropDisc(int column, int &row) {
    for (int r = rows -1; r >= 0; --r) {
        if (grid[r][column] == 0) {
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

    repaint();

}


static QColor get_opposite_color(QColor color)
{
    int r = 255 - color.red();
    int g = 255 - color.green();
    int b = 255 - color.blue();

    return QColor(r, g, b);
}



void GameBoard::receive_current_players(int num)
{
    switch(num)
    {
    case 0:
        curr_state = disable;
        break;
    case 1:
        curr_state = player1;
        cpu_on = true;
        emit turnChanged(0);
        break;
    case 2:
        curr_state = player2;
        cpu_on = false;
        break;
    }

    resetBoard();
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
