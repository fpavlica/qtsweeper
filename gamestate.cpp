#include "gamestate.h"
#include <algorithm>
#include <random>
//#include <iostream>


//should probs move this into the constructor
void GameState::setUp(int height, int width, long numMines){
    gameGrid = QVector<QVector<Tile>>(height);
//    gameGrid = std::vector<std::vector<Tile>>(height);
    for (auto& row : gameGrid){
        row = QVector<Tile>(width);
//        row = std::vector<Tile>(width);
        for (auto& tile: row) {
            tile.containsMine = false;
            tile.flagMarked = false;
            tile.opened = false;
        }
    }
//    openedGrid = std::vector<std::vector<bool>>(height);
//    for (auto& row : openedGrid){
//        row = std::vector<bool>(width, false);
//    }

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
            int row = i / width;
            int col = i % width;

            gameGrid[row][col].containsMine = true;
        }
    }
}

char GameState::countNeighbours(int row, int col) {
    char mineCount = 0;
    for (int rowi = row - 1; rowi <= row + 1; ++rowi) {
        if (rowi < 0 || rowi >= height) continue;

        for (int coli = (int)col - 1; coli <= (int)col + 1; ++coli) {
            if (coli < 0 || coli >= width) continue;

            if (gameGrid.at(rowi).at(coli).containsMine == true) {
                ++mineCount;
            }
        }
    }
    return mineCount;
}

void GameState::openSurrounding(int row, int col) {
    for (int rowi = row - 1; rowi <= row + 1; ++rowi) {
        if (rowi < 0 || rowi >= height) continue;

        for (int coli = (int)col - 1; coli <= (int)col + 1; ++coli) {
            if (coli < 0 || coli >= width) continue;
            if (coli == col && rowi == row) continue; //skip the centre tile

            openTile(rowi, coli);
        }
    }
}

void GameState::openTile(int row, int col) {
    Tile& tile = gameGrid[row][col]; //do NOT use .at() here if using QVectors. That fn returns a const value and cannot be stored as a reference, unlike with STL .at().
    if (tile.opened)
        return; //do not open again once opened

    //increment numOpened
    ++numOpened;
    tile.opened = true;
    //first explode if needed.
    if (tile.containsMine) {
//        return 9;
        emit tileRevealed(row, col, 9);
        emit gameFinished(false);
    } else {
        //no mine on current tile
        //check surrounding tiles for number of mines
        char mineCount = countNeighbours(row, col);
        emit tileRevealed(row, col, mineCount);

        //if mineCount is still 0, surrounding tiles should be opened too
        if (mineCount == 0) {
            //open surrounding tiles
            openSurrounding(row, col);
        }

        if (isGameWon()) {
            //TODO currently if this happens on a multi-opening, this signal gets emitted multiple times
            emit gameFinished(true);
        }
    }
}

bool GameState::toggleFlag(int row, int col) {
    Tile& tile = gameGrid[row][col];
    if (tile.opened)
        return false; //do not allow flagging opened tiles.

    tile.flagMarked = !tile.flagMarked;
    emit flagToggled(row, col, tile.flagMarked);
    return tile.flagMarked;
}

//maybe have this as a game state enum instead? (won, lost, in_progress)
//also, this does not account for if a tile has been opened twice in one game
bool GameState::isGameWon() {
    return numOpened + numMines >= width * height;
}
