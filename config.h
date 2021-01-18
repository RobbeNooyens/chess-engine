//
// Created by robbe on 15-1-2021.
//

#ifndef SCHAKEN_CONFIG_H
#define SCHAKEN_CONFIG_H

#include "sharedtypes.h"

class GameConfig {
public:
    const ZW botColor = zwart;
    const bool enableBot = true;
    const bool showCheckPopup = false, showMatePopup = true;
    const bool visualizeOwnThreatenedPieces = true, visualizeOpponentThreatenedPieces = false;
    const BoardLayoutType boardLayoutType = standard;
    const BoardLayout& getBoardSetup() {
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
        {{' ', ' ', ' ', ' ', ' ', 'n', ' ', 'K'},
         {' ', ' ', ' ', ' ', ' ', ' ', 'r', ' '},
         {' ', ' ', 'P', ' ', ' ', ' ', ' ', 'P'},
         {' ', 'P', 'p', ' ', ' ', ' ', 'P', 'k'},
         {' ', 'p', ' ', ' ', 'p', ' ', 'p', ' '},
         {' ', ' ', ' ', 'p', ' ', ' ', ' ', 'p'},
         {' ', ' ', ' ', ' ', ' ', ' ', 'R', ' '},
         {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};
};

#endif //SCHAKEN_CONFIG_H
