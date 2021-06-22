#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadGameIcons();
    this->placeGameButtons(8, 10);
    numMines = 10;
    game.setUp(8, 10, numMines);

    connect(ui->bRestart, &QPushButton::clicked, this, &MainWindow::onRestartClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadGameIcons(){
    this->gameIcons = QVector<QPixmap>(12);
    for (char i = 0; i <= 8; ++i) {
        QString fname = QString("icons/ico_") + QString((char)(i + '0')) + QString("_128.png");
        qDebug() << "Loading icon " << fname;
        gameIcons[i] = QPixmap(fname);
    }
    // 9 will be bomb
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

    QSize buttSize(64, 64);

    //set grid size to get rid of spaces between buttons
    QSize gridSize(buttSize.width() * gridWidth, buttSize.height() * gridHeight);
    gg->setMaximumSize(gridSize);
    gg->setMinimumSize(gridSize);
    QSize windowSize(gridSize.width() * 1.05, gridSize.height() * 1.35);
//    this->setMaximumSize(windowSize);
    this->setFixedSize(windowSize);

    //create all the buttons
    for (int row = 0; row < height; ++row) {
        for (int col =0; col < width; ++col) {
            QMineButton* gridButton = new QMineButton(row, col, gg);

            gridVector[row][col] = gridButton;
            //icon colour RGBs are 37 52 84 dec or 25 34 54 hex
            gridButton->setIcon(QIcon(gameIcons[10]));
//            gridButton->setIcon(QIcon("icons/ico_unopened.png"));
            QSize siz(64,64);
            gridButton->setIconSize(siz);
            gridButton->setMaximumSize(siz);
            gridButton->setMinimumSize(siz);
            //QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
            //gridButton->setStyleSheet(buttonStyle); // Style sheet

            ggl->addWidget(gridButton, row, col);

            connect(gridButton, &QMineButton::rightPressed,
                    this, &MainWindow::onMineRightPressed);


            connect(gridButton, &QMineButton::leftPressed,
                    this, &MainWindow::onMineLeftPressed);

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

    if (mb->hasBeenOpened())
        return; //do not allow marking opened tiles

//    mb->setText(QString('x'));
    if (mb->flag_marked) {
        //this should maybe be done in an accessor function instead.
        mb->flag_marked = false;
        mb->setIcon(gameIcons[10]);
    }
    else{
        mb->flag_marked = true;
        mb->setIcon(gameIcons[11]);
    }
}

void MainWindow::onMineLeftPressed() {
    QMineButton* mb = qobject_cast<QMineButton*>(sender());
    qDebug() << "left clicked: (" << mb->getRow() << ", " << mb->getCol() << ").";
    openTile(mb);
}

void MainWindow::openTile(QMineButton *mb) {
    if(mb->hasBeenOpened())
        return; //do not open what has already been opened
    if (mb->flag_marked)
        return; //do not open if marked.

    char tileval = game.openSingleTile(mb->getRow(), mb->getCol());
    qDebug() << "value: " << (int)tileval;
//    char valchar = val0 + '0';

//    mb->setText(QString(valchar));
    mb->setIcon(gameIcons[tileval]);
    mb->setOpened(true);
    if (tileval == 0) {
        // open all adjacent tiles
        openAdjacentTiles(mb);
    } else if (tileval == 9) {
//        qDebug() << "oopsie boom boom";
        QMessageBox mbox;
        mbox.setText("You lost");
        ui->wGameGrid->setEnabled(false);
//        ui->wGameGrid->blockSignals(true);
        mbox.exec();
    } else if (game.isGameWon()) {
//        qDebug() <<"yay you won";
        QMessageBox mbox;
        mbox.setText("You win!");
        ui->wGameGrid->setEnabled(false);
        mbox.exec();
    }

    //lock the board after a win or lose.


}

//recursively opens 8 adjacent tiles to centre.
void MainWindow::openAdjacentTiles(QMineButton *centre) {
    int row = centre->getRow();
    int col = centre->getCol();

    for (int rowi = (int)row - 1; rowi <= (int)row + 1; ++rowi) {
        if (rowi < 0 || rowi >= (int)gridHeight) continue;

        for (int coli = (int)col - 1; coli <= (int)col + 1; ++coli) {
            if (coli < 0 || coli >= (int)gridWidth) continue;
            if (rowi == row && coli == col) continue; //skip the centre tile

            QMineButton* mb = gridVector[rowi][coli];
            //clear flag mark if opening a 0-area
            mb->flag_marked = false;
            openTile(mb);
        }
    }
}
