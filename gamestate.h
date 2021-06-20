#ifndef GAMESTATE_H
#define GAMESTATE_H

//could use QVectors instead. Using std vectors in case I want to reuse this code elsewhere
//could also use a hash set of mine location indices if memory is a concern
#include <vector>

class GameState
{
public:
    GameState();

    void setUp(unsigned int height, unsigned int width, unsigned long numMines);

    char openSingleTile(unsigned int row, unsigned int col);

    bool isGameWon();

private:
    std::vector<std::vector<bool>> gameGrid;
//    void placeMines(unsigned int height, unsigned int width, unsigned int numMines);
    unsigned long numOpened, numMines;

    unsigned int width, height;
};

#endif // GAMESTATE_H
