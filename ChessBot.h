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
    int ai_count_empty_start_tiles(ZW) const;
    int ai_sum_of_threatened_pieces(ZW) const;
    bool ai_resolve_check(ZW);
    bool ai_move_piece(Move) const;
    bool ai_move_leads_to_mate(Move) const;
    bool ai_move_leads_to_check(Move, ZW) const;
    bool ai_resolve_threatened_piece(SchaakStuk*);
    bool ai_can_take_attacker(SchaakStuk*, SchaakStuk*) const;
    bool ai_piece_covered(SchaakStuk*) const;
    bool ai_safe_move(Move) const;
    bool debugBot_ = false;
    ChessBoard* scene_;
    Game* game_;
};


#endif //SCHAKEN_CHESSBOT_H
