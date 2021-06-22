#ifndef QMINEBUTTON_H
#define QMINEBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class QMineButton : public QPushButton
{
    Q_OBJECT

public:
    QMineButton(unsigned int row, unsigned int col, QWidget *parent = nullptr);
//    explicit QMineButton(QWidget *parent = nullptr);

    unsigned int getRow();
    unsigned int getCol();
    bool hasBeenOpened();
    void setOpened(bool opened);
    bool flag_marked = false;

private:
    unsigned int row;
    unsigned int col;
    bool opened = false;

//    bool leftPressed = false, rightPressed = false;

private slots:
    void mousePressEvent(QMouseEvent *e);
//    void mouseReleaseEvent(QMouseEvent *e);
//    void mouseMoveEvent(QMouseEvent *e);

signals:
    void leftPressed();
    void rightPressed();

//    void rightClicked();
//    void leftClicked(); // same as what QAbstractButton::clicked() does
};

#endif // QMINEBUTTON_H
