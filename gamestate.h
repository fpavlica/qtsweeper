#ifndef GAMESTATE_H
#define GAMESTATE_H

//could use QVectors instead. Using std vectors in case I want to reuse this code elsewhere
//could also use a hash set of mine location indices if memory is a concern
#include <vector>
#include <QObject>

class GameState  : public QObject
{
    Q_OBJECT

public:

    void setUp(int height, int width, long numMines);

    void openTile(int row, int col);

    bool isGameWon();

private:
    std::vector<std::vector<bool>> gameGrid;
    std::vector<std::vector<bool>> openedGrid;
//    void placeMines(unsigned int height, unsigned int width, unsigned int numMines);
    long numOpened, numMines;

    int width, height;

    char countNeighbours(int row, int col);
    void openSurrounding(int row, int col);

signals:
    void tileRevealed(int grid_row, int grid_col, char value);
    void gameFinished(bool win);

};

#endif // GAMESTATE_H
