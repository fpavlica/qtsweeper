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
    GameState game;
    QVector<QPixmap> gameIcons;
    void loadGameIcons();

private slots:
    void onMineRightReleased();
    void onMineLeftReleased();
};
#endif // MAINWINDOW_H
