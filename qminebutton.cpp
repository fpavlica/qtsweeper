#include "qminebutton.h"

QMineButton::QMineButton(unsigned int row, unsigned int col, QWidget *parent) :
    QPushButton(parent)
{
    this->row = row;
    this->col = col;
}

unsigned int QMineButton::getCol() { return col; }
unsigned int QMineButton::getRow() { return row; }

void QMineButton::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) emit rightReleased();
    else QPushButton::mouseReleaseEvent(e);
}
