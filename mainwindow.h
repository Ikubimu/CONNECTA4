#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "gameboard.h"
#include "rankingtable.h"
#include "userswidget.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameBoard board;
    rankingTable rank;
    usersWidget userL, userR;
    QWidget* centralWidget;


    QWidget* buildMainWidget();
    void catchSignal();
};
#endif // MAINWINDOW_H
