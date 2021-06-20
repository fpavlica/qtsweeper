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

private:
    unsigned int row;
    unsigned int col;

private slots:
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void rightReleased();
};

#endif // QMINEBUTTON_H
