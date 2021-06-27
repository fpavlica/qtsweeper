#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qminebutton.h"
#include "gamestate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void placeGameButtons(int height = 10, int width = 8);
    void clearGameButtons();
    QVector<QVector<QMineButton*>> gridVector;
    unsigned int gridWidth;
    unsigned int gridHeight;
    unsigned long numMines;
    GameState game;
    QVector<QPixmap> gameIcons;
    void loadGameIcons();

    void openTile(QMineButton *mb);

private slots:
    void onMineRightPressed();
    void onMineLeftPressed();

    void onTileRevealed(int grid_row, int grid_col, int val);
    void onGameFinished(bool win);

    void onRestartClicked();

};
#endif // MAINWINDOW_H
