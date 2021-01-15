//
// Created by robbe on 15-1-2021.
//

#ifndef SCHAKEN_CONFIG_H
#define SCHAKEN_CONFIG_H

#include "SchaakStuk.h"
#include "game.h"

typedef char BoardLayout[8][8];

enum BoardLayoutType{standard, endgame, promote, rokade};

class GameConfig {
public:
    const BoardLayoutType boardLayoutType = standard;
    const BoardLayout& getSetup() {
        switch (boardLayoutType) {
            case endgame:
                return endgameSetup;
            case standard:
            default:
                return standardSetup;
        }
    }
private:
    const BoardLayout standardSetup =
        {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
         {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
         {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};
    const BoardLayout endgameSetup =
        {{' ', ' ', ' ', 'Q', 'K', 'Q', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
         {' ', ' ', ' ', ' ', 'k', ' ', ' ', ' '}};
};

#endif //SCHAKEN_CONFIG_H