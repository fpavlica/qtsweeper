#include "qminebutton.h"

QMineButton::QMineButton(unsigned int row, unsigned int col, QWidget *parent) :
    QPushButton(parent)
{
    this->row = row;
    this->col = col;
}

unsigned int QMineButton::getCol() { return col; }
unsigned int QMineButton::getRow() { return row; }
bool QMineButton::hasBeenOpened()  { return opened; }

void QMineButton::setOpened(bool opened) { this->opened = opened; }

void QMineButton::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) emit leftPressed();
    else if (e->button() == Qt::RightButton) emit rightPressed();
//    if (e->button() == Qt::LeftButton) leftPressed = true;
//    else if (e->button() == Qt::RightButton) rightPressed = true;
//    else QPushButton::mouseReleaseEvent(e);
}

//void QMineButton::mouseMoveEvent(QMouseEvent *e) {
//    //dragging; unpress all buttons.
//    leftPressed = false;
//    rightPressed = false;
////    QPushButton::mouseMoveEvent(e); //
//}

//void QMineButton::mouseReleaseEvent(QMouseEvent *e) {
//    if (e->button() == Qt::LeftButton && this->leftPressed) {
////        if (this->underMouse()) {
//        leftPressed = false;
//        emit leftClicked();
////        }
//    }
//    else if (e->button() == Qt::RightButton && this->rightPressed) {
//        rightPressed = false;
//        emit rightClicked();
//    }
////    else QPushButton::mouseReleaseEvent(e);
//}
