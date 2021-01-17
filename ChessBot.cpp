//
// Created by robbe on 17-1-2021.
//

#include <exceptions/BotCantResolveCheck.h>
#include "ChessBot.h"
#include "game.h"
#include "SchaakStuk.h"

ChessBot::ChessBot(Game* game, ChessBoard* scene):game_(game), scene_(scene) {}

bool ChessBot::ai_move(ZW color) {
    Game::pointerRequireNonNull(game_);
    Game::pointerRequireNonNull(scene_);
    // Check if own king is checked
    if(game_->check(color))
        return ai_resolve_check(color);
    // Check for mate
    for(const auto &piece: game_->get_pieces_of_color(color))
        for(const auto &move: piece->valid_moves(game_))
            if(ai_move_leads_to_mate({piece, move}))
                return ai_move_piece({piece, move});
    // Check for check
    for(int i = 1; i <= 9; i++)
        for(const auto &piece: game_->get_pieces_with_numeric_value(color, i))
            for(const auto &move: piece->valid_moves(game_))
                if(piece->is_safe_move(game_, move) && ai_move_leads_to_check({piece, move}))
                    return ai_move_piece({piece, move});
    // Check if any high value piece is being attacked
    Tiles threats = game_->get_threatened_tiles(Game::opposite(color));
    for(int i = 9; i >= 2; i++){
        for(const auto &piece: game_->get_pieces_with_numeric_value(color, i)){
            if(Game::vector_contains_tile(threats, piece->get_position())){
                // Check if a smaller own piece can take the attacker
                Move defense = ai_can_take_attacker(piece);
                if(defense.first != nullptr)
                    return game_->move(defense.first, defense.second);
                // Check if the piece can flee to a safe field
                for(const auto &move: piece->valid_moves(game_))
                    if(piece->is_safe_move(game_, move))
                        return game_->move(piece, move);
            }
        }
    }
    // Check if bot can take an enemy piece
    bool foundTarget = false;
    SchaakStuk* ownPiece = nullptr;
    Tile targetTile = {};
    int numericDifference = 0;
    for(int i = 1; i <= 9; i++){
        for(const auto &piece: game_->get_pieces_with_numeric_value(color, i)){
            for(const auto &move: piece->valid_moves(game_)){
                SchaakStuk* target = game_->get_piece(move);
                if(target == nullptr)
                    continue;
                int difference = std::abs(piece->get_numeric_value() - target->get_numeric_value());
                if(target->get_numeric_value() >= i && difference >= numericDifference){
                    foundTarget = true;
                    ownPiece = piece;
                    targetTile = target->get_position();
                }
            }
        }
    }
    if(foundTarget)
        return ai_move_piece({ownPiece, targetTile});
    // Move random piece
    int attackingTiles = 0;
    Move currentBestMove;
    for(const auto &piece: game_->get_pieces_of_color(color)){
        for(const auto &move: piece->valid_moves(game_)){
            int tiles = ai_count_tiles_after_move({piece, move});
            if(tiles > attackingTiles){
                currentBestMove = {piece, move};
                attackingTiles = tiles;
            }
        }
    }
    return ai_move_piece(currentBestMove);

}

Move ChessBot::ai_can_take_attacker(SchaakStuk* piece) {
    Game::pointerRequireNonNull(piece);
    for(const auto &attacker: game_->get_pieces_of_color(Game::opposite(piece->get_color())))
        if(Game::vector_contains_tile(piece->valid_moves(game_), piece->get_position()))
            for(int i = 1; i <= attacker->get_numeric_value(); i++)
                for(const auto &defender: game_->get_pieces_with_numeric_value(piece->get_color(), i))
                    if(Game::vector_contains_tile(defender->valid_moves(game_), attacker->get_position()))
                        return {defender, attacker->get_position()};
    return {nullptr, {}};
}

bool ChessBot::ai_resolve_check(ZW color) {
    SchaakStuk* koning = game_->find_king(color);
    Tiles threats = game_->get_tiles_to_king(koning);
    for(int i = 1; i <= 9; i++)
        for(const auto &piece: game_->get_pieces_with_numeric_value(color, i))
            for(const auto &move: piece->valid_moves(game_))
                if(Game::vector_contains_tile(threats, move) && ai_move_piece({koning, move}))
                    return true;
    throw BotCantResolveCheck(color);
}

bool ChessBot::ai_move_piece(Move move){
    Game::pointerRequireNonNull(move.first);
    if(!game_->move(move.first, move.second))
        return false;
    game_->piece_moved(scene_, move.second, game_->is_enpassant(move.first, move.second));
    return true;
}

bool ChessBot::ai_move_leads_to_mate(Move move) {
    Game::pointerRequireNonNull(move.first);
    std::string backup = game_->save();
    game_->move(move.first, move.second);
    bool checkmate = game_->checkmate(Game::opposite(move.first->get_color()));
    game_->load(scene_, backup, false);
    return checkmate;
}

bool ChessBot::ai_move_leads_to_check(Move move) {
    Game::pointerRequireNonNull(move.first);
    std::string backup = game_->save();
    game_->move(move.first, move.second);
    bool check = game_->check(Game::opposite(move.first->get_color()));
    game_->load(scene_, backup, false);
    return check;
}

int ChessBot::ai_count_tiles_after_move(Move move) {
    Game::pointerRequireNonNull(move.first);
    std::string backup = game_->save();
    game_->move(move.first, move.second);
    int count = game_->get_threatened_tiles(move.first->get_color()).size();
    game_->load(scene_, backup, false);
    return count;
}
