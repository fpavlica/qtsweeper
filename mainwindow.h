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

    QMineButton* makeButton(QSize buttonSize, int row, int col);
    void setUpWindowSize(QSize buttonSize);
    void placeGameButtons(int height = 10, int width = 8);
    void clearGameButtons();
    QVector<QVector<QMineButton*>> gridVector;
    unsigned int gridWidth;
    unsigned int gridHeight;
    unsigned long numMines;
    GameState game;
    QVector<QPixmap> gameIcons;
    void loadGameIcons();

private slots:
    void onMineRightPressed();
    void onMineLeftPressed();

    void onTileRevealed(int grid_row, int grid_col, int val);
    void onGameFinished(bool win);
    void onFlagToggled(int grid_row, int grid_col, bool newState);

    void onRestartClicked();

};
#endif // MAINWINDOW_H
