#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>

//a lot of the functionality in here should be moved to GameState; communicating with signals to update the board.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadGameIcons();
    int width = 8, height = 10;
    this->numMines = 10;

    this->placeGameButtons(height, width);
    game.setUp(height, width, numMines);

    connect(ui->bRestart, &QPushButton::clicked, this, &MainWindow::onRestartClicked);
    connect(&game, &GameState::tileRevealed, this, &MainWindow::onTileRevealed);
    connect(&game, &GameState::gameFinished, this, &MainWindow::onGameFinished);
    connect(&game, &GameState::flagToggled, this, &MainWindow::onFlagToggled);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadGameIcons(){
    this->gameIcons = QVector<QPixmap>(12);
    for (char i = 0; i <= 8; ++i) {
        QString fname = QString("icons/ico_") + QString((char)(i + '0')) + QString("_128.png");
        gameIcons[i] = QPixmap(fname);
    }
    // 9 will be the bomb
    gameIcons[9] = QPixmap("icons/ico_bomb_128.png");
    // 10 will be empty
    gameIcons[10] = QPixmap("icons/ico_unopened_128.png");
    // 11 will be flag
    gameIcons[11] = QPixmap("icons/ico_flag_128.png");

    for (int i = 0; i< gameIcons.size(); ++i) {
        if (gameIcons[i].isNull()){
            qDebug() << "Could not load icons.";
            qDebug() << "current directory is " << QDir::currentPath();
            break;
        }
    }
}

QMineButton* MainWindow::makeButton(QSize buttonSize, int row, int col) {
    QMineButton* gridButton = new QMineButton(row, col, ui->wGameGrid);

    //icon colour RGBs are 37 52 84 dec or 25 34 54 hex
    gridButton->setIcon(QIcon(gameIcons[10]));
    gridButton->setIconSize(buttonSize);
    gridButton->setMaximumSize(buttonSize);
    gridButton->setMinimumSize(buttonSize);

    connect(gridButton, &QMineButton::rightPressed,
            this, &MainWindow::onMineRightPressed);


    connect(gridButton, &QMineButton::leftPressed,
            this, &MainWindow::onMineLeftPressed);

    return gridButton;
}

void MainWindow::setUpWindowSize(QSize buttonSize) {
    //grid size in pixels is fixed by the number of tiles and the size of a tile.
    //this is done to get rid of spaces between buttons - I didn't figure out how to set the padding/margins to 0.
    QSize gridSize(buttonSize.width() * gridWidth, buttonSize.height() * gridHeight);
    ui->wGameGrid->setMaximumSize(gridSize);
    ui->wGameGrid->setMinimumSize(gridSize);
    //window size is also fixed.
    QSize windowSize(gridSize.width() * 1.05, gridSize.height() * 1.35);
    this->setFixedSize(windowSize);
}

void MainWindow::placeGameButtons(int height, int width) {
    QWidget* gg = ui->wGameGrid;
    QGridLayout* ggl = qobject_cast<QGridLayout*> (gg->layout());

    //set up my holding vector. maybe move this elsewhere
    gridVector = QVector<QVector<QMineButton*>>(height);
    gridHeight = height;
    gridWidth = width;
    for (auto& row : gridVector){
        row = QVector<QMineButton*>(width);
    }

    QSize buttonSize(64, 64);

    //set grid size to get rid of spaces between buttons
    setUpWindowSize(buttonSize);

    //create all the buttons
    for (int row = 0; row < height; ++row) {
        for (int col =0; col < width; ++col) {
            QMineButton* gridButton = makeButton(buttonSize, row, col);

            gridVector[row][col] = gridButton;
            ggl->addWidget(gridButton, row, col);
            gridButton->show();
        }
    }
}

//untested
void MainWindow::clearGameButtons(){
    QGridLayout* ggl = qobject_cast<QGridLayout*> (ui->wGameGrid->layout());
    while(ggl->count() != 0){
        QLayoutItem* item = ggl->takeAt(0);
        delete item->widget();
        delete item;
    }
    gridVector = QVector<QVector<QMineButton*>>();
}

void MainWindow::onRestartClicked() {
    clearGameButtons();
    ui->wGameGrid->setEnabled(true);
    placeGameButtons(gridHeight, gridWidth);
    game.setUp(gridHeight, gridWidth, numMines);
}

void MainWindow::onMineRightPressed() {
    QMineButton* mb = qobject_cast<QMineButton*>(sender());
    qDebug() << "right clicked: (" << mb->getRow() << ", " << mb->getCol() << ").";
    game.toggleFlag(mb->getRow(), mb->getCol());

}

void MainWindow::onFlagToggled(int grid_row, int grid_col, bool newState){
    gridVector[grid_row][grid_col]->setIcon(gameIcons[newState == true? 11 : 10]);
}

void MainWindow::onMineLeftPressed() {
    QMineButton* mb = qobject_cast<QMineButton*>(sender());
    qDebug() << "left clicked: (" << mb->getRow() << ", " << mb->getCol() << ").";
    game.openTile(mb->getRow(), mb->getCol());
}

void MainWindow::onTileRevealed(int grid_row, int grid_col, int val) {
    auto * mb = gridVector.at(grid_row).at(grid_col);
    mb->setIcon(gameIcons[val]);
}

void MainWindow::onGameFinished(bool win) {
    //win signal received <- all non-mine tiles opened
    //lose signal received <- mine opened
    QMessageBox mbox;
    mbox.setText(win? "You win!" : "You lost");
    ui->wGameGrid->setEnabled(false);
    mbox.exec();
}
