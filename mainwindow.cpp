#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadGameIcons();
    this->placeGameButtons(8, 10);
    game.setUp(8, 10, 10);

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
    for (auto& row : gridVector){
        row = QVector<QMineButton*>(width);
    }

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

            connect(gridButton, &QMineButton::rightReleased,
                    this, &MainWindow::onMineRightReleased);


            connect(gridButton, &QPushButton::released,
                    this, &MainWindow::onMineLeftReleased);

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

void MainWindow::onMineRightReleased() {
    QMineButton* mb = qobject_cast<QMineButton*>(sender());
    qDebug() << "right clicked: (" << mb->getRow() << ", " << mb->getCol() << ").";

//    mb->setText(QString('x'));
    mb->setIcon(gameIcons[11]);
}

void MainWindow::onMineLeftReleased() {
    QMineButton* mb = qobject_cast<QMineButton*>(sender());
    qDebug() << "left clicked: (" << mb->getRow() << ", " << mb->getCol() << ").";

    char val0 = game.openSingleTile(mb->getRow(), mb->getCol());
    qDebug() << "value: " << (int)val0;
    char valchar = val0 + '0';

//    mb->setText(QString(valchar));
    mb->setIcon(gameIcons[val0]);

    if (val0 == 9) {
        qDebug() << "oopsie boom boom";
    } else if (game.isGameWon()) {
        qDebug() <<"yay you won";
    }
}
