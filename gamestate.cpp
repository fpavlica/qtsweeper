#include "gamestate.h"
#include <algorithm>
#include <random>
//#include <iostream>

GameState::GameState()
{

}

//should probs move this into the constructor
void GameState::setUp(unsigned int height, unsigned int width, unsigned long numMines){
    gameGrid = std::vector<std::vector<bool>>(height);
    for (auto& row : gameGrid){
        row = std::vector<bool>(width, false);
    }

    numOpened = 0;
    this->width = width;
    this->height = height;
    this->numMines = numMines;

    //placing mines
    {
        //these will be the indices of the mines on a flattened grid
        std::vector<unsigned int> indices;

        //like python range. Not efficient but doesn't matter
        std::vector<unsigned int> range(width * height);
        std::iota(range.begin(), range.end(), 0);

        //random sample from range
        std::sample(range.begin(), range.end(), std::back_inserter(indices), numMines,
                    std::mt19937{std::random_device{}()});

        //converting from flat indices to placed mines on the game grid.
        for (const unsigned int& i: indices) {
            unsigned int row = i / width;
            unsigned int col = i % width;

            gameGrid[row][col] = true;
        }
    }
}

char GameState::openSingleTile(unsigned int row, unsigned int col) {
    //increment numOpened
    ++numOpened;
    //first explode if needed.
    if (gameGrid[row][col] == true) {
        return 9;
    } else {
        //no mine on current tile
        //check surrounding tiles for number of mines
        int mineCount = 0;
        for (int rowi = (int)row - 1; rowi <= (int)row + 1; ++rowi) {
            if (rowi < 0 || rowi >= gameGrid.size()) continue;

            for (int coli = (int)col - 1; coli <= (int)col + 1; ++coli) {
                if (coli < 0 || coli >= gameGrid[rowi].size()) continue;

//                std::cout << "checking rowi " << rowi << ", coli " <<coli <<std::endl;
//                mineCount += gameGrid[rowi][coli]; //true is +1, false nothing
                if (gameGrid[rowi][coli] == true) {
                    ++mineCount;
                }
            }
        }
        //if mineCount is still 0, surrounding tiles should be opened too
        return mineCount;
    }
}

//maybe have this as a game state enum instead (won, lost, in_progress)
bool GameState::isGameWon() {
    return numOpened + numMines >= width * height;
}
