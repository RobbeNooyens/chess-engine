//
// Created by robbe on 17-1-2021.
//

#ifndef SCHAKEN_CHESSBOT_H
#define SCHAKEN_CHESSBOT_H

#include "sharedtypes.h"

class ChessBot {
public:
    ChessBot(Game*, ChessBoard*);
    void ai_move(ZW);
    void ai_resolve_check(ZW);
    bool ai_move_piece(SchaakStuk*, Tile);
    bool ai_move_leads_to_mate(SchaakStuk*, Tile);

private:
    ChessBoard* scene_;
    Game* game_;
};


#endif //SCHAKEN_CHESSBOT_H
