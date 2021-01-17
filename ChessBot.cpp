//
// Created by robbe on 17-1-2021.
//

#include <exceptions/BotCantResolveCheck.h>
#include "ChessBot.h"
#include "game.h"
#include "SchaakStuk.h"

// AI Bot

ChessBot::ChessBot(Game* game, ChessBoard* scene):game_(game), scene_(scene) {}

void ChessBot::ai_move(ZW color) {
    // Check if own king is checked
    if(game_->check(color))
        ai_resolve_check(color);
    // Check for mate
    for(const auto &piece: game_->get_pieces_of_color(color))
        for(const auto &move: piece->valid_moves(game_))
            continue;
    // Check for check


}

void ChessBot::ai_resolve_check(ZW color) {
    SchaakStuk* koning = game_->find_king(color);
    Tiles threats = game_->get_tiles_to_king(koning);
    for(int i = 1; i <= 9; i++)
        for(const auto &piece: game_->get_pieces_with_numeric_value(color, i))
            for(const auto &move: piece->valid_moves(game_))
                if(Game::vector_contains_tile(threats, move) && ai_move_piece(koning, move))
                    return;
    throw BotCantResolveCheck(color);
}

bool ChessBot::ai_move_piece(SchaakStuk* piece, Tile position){
    if(!game_->move(piece, position))
        return false;
    game_->piece_moved(scene_, position, game_->is_enpassant(piece, position));
    return true;
}

bool ChessBot::ai_move_leads_to_mate(SchaakStuk* piece, Tile position) {
    std::string backup = game_->save();
    game_->move(piece, position);
    game_->load(scene_, backup, false);
    return false;
}
