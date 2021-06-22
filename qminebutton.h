#ifndef QMINEBUTTON_H
#define QMINEBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class QMineButton : public QPushButton
{
    Q_OBJECT

public:
    QMineButton(unsigned int row, unsigned int col, QWidget *parent = nullptr);
    unsigned int getRow();
    unsigned int getCol();
    bool isOpened();
    void setOpened(bool opened);
    bool isFlagMarked() { return flag_marked; }
    void setFlagMarked(bool marked) { this->flag_marked = marked; }

private:
    unsigned int row;
    unsigned int col;
    bool opened = false;
    bool flag_marked = false;

private slots:
    void mousePressEvent(QMouseEvent *e);

signals:
    void leftPressed();
    void rightPressed();
};

#endif // QMINEBUTTON_H
