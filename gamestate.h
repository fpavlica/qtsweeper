#ifndef GAMESTATE_H
#define GAMESTATE_H

//could use QVectors instead. Using std vectors in case I want to reuse this code elsewhere
//could also use a hash set of mine location indices if memory is a concern
#include <vector>
#include <QObject>
#include <QVector>

class GameState  : public QObject
{
    Q_OBJECT

public:
    struct Tile {
        bool containsMine = false;
        bool opened = false;
        bool flagMarked = false;
    };

    void setUp(int height, int width, long numMines);

    void openTile(int row, int col);

    bool isGameWon();
    bool toggleFlag(int row, int col); //returns true if newly flagged, false if newly unflagged

private:
    QVector<QVector<Tile>> gameGrid;
    long numOpened = 0, numMines = 0;

    int width = 0, height = 0;

    void placeMines(); //uses pre-set values
    void placeMines(int height, int width, int numMines);
    char countNeighbours(int row, int col);
    void openSurrounding(int row, int col);

signals:
    void tileRevealed(int grid_row, int grid_col, char value);
    void gameFinished(bool win);
    void flagToggled(int grid_row, int grid_col, bool newState);

};

#endif // GAMESTATE_H
