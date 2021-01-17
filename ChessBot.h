//
// Created by robbe on 17-1-2021.
//

#ifndef SCHAKEN_CHESSBOT_H
#define SCHAKEN_CHESSBOT_H

#include "sharedtypes.h"

typedef std::pair<SchaakStuk*, Tile> Move;

class ChessBot {
public:
    ChessBot(Game*, ChessBoard*);
    bool ai_move(ZW);
private:
    void debug(const std::string&, int=0) const;
    bool ai_resolve_check(ZW);
    bool ai_move_piece(Move);
    bool ai_move_leads_to_mate(Move);
    bool ai_move_leads_to_check(Move, ZW);
    int ai_count_tiles_after_move(Move);
    int ai_count_empty_start_tiles(ZW);
    int ai_sum_of_threatened_pieces(ZW);
    Move ai_can_take_attacker(SchaakStuk*);
    ChessBoard* scene_;
    Game* game_;
    bool debugBot_ = false;
};


#endif //SCHAKEN_CHESSBOT_H
