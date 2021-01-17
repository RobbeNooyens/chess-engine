//
// Created by robbe on 17-1-2021.
//

#include <exceptions/BotCantResolveCheck.h>
#include <iostream>
#include "ChessBot.h"
#include "game.h"
#include "SchaakStuk.h"
#include "MoveSimulation.h"

ChessBot::ChessBot(Game* game, ChessBoard* scene):game_(game), scene_(scene) {}

bool ChessBot::ai_move(ZW color) {
    Game::pointerRequireNonNull(game_);
    Game::pointerRequireNonNull(scene_);
    ChessBot::debug("============[ ChessBot is trying to find the best move ]===========");
    // Check if own king is checked
    ChessBot::debug("Checking if own king is checked");
    if(game_->check(color))
        return ai_resolve_check(color);
    ChessBot::debug("Own king isn't checked", 1);
    // Check for mate
    ChessBot::debug("Checking if mating opponent is possible");
    for(SchaakStuk* piece: game_->get_pieces_of_color(color))
        for(const auto &move: piece->valid_moves(game_))
            if(ai_move_leads_to_mate({piece, move}))
                return ai_move_piece({piece, move});
    ChessBot::debug("Mating opponent isn't possible", 1);
    // Check for check
    ChessBot::debug("Checking if it's possible to check opponents king");
    for(int i = 1; i <= 9; i++)
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i))
            for(const auto &move: piece->valid_moves(game_))
                if(piece->is_safe_move(game_, move) && ai_move_leads_to_check({piece, move}))
                    return ai_move_piece({piece, move});
    ChessBot::debug("It's not possible to safely check opponents king", 1);
    // Check if any high value piece is being attacked
    ChessBot::debug("Checking if any high value piece is being threatened");
    Tiles threats = game_->get_threatened_tiles(Game::opposite(color));
    for(int i = 9; i >= 2; i--){
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i)){
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
    ChessBot::debug("No high value piece is being threathened or can escape safely", 1);
    // Check if bot can take an enemy piece
    ChessBot::debug("Checking if bot can take one of opponents pieces");
    bool foundTarget = false;
    SchaakStuk* ownPiece = nullptr;
    Tile targetTile = {};
    int numericDifference = 0;
    for(int i = 1; i <= 9; i++){
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i)){
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
    ChessBot::debug("Can't taken an opponents piece safely", 1);
    // Move random piece
    ChessBot::debug("Searching for the best random move");
    int attackingTiles = 0;
    Move currentBestMove;
    for(SchaakStuk* piece: game_->get_pieces_of_color(color)){
        for(const auto &move: piece->valid_moves(game_)){
            if(!piece->is_safe_move(game_, move))
                continue;
            int tiles = ai_count_tiles_after_move({piece, move});
            if(tiles > attackingTiles){
                currentBestMove = {piece, move};
                attackingTiles = tiles;
            }
        }
    }
    std::cout << "Bot has chosen a random move: moving " << currentBestMove.first->get_row() << "," <<
    currentBestMove.first->get_column() << " to " << currentBestMove.second.first << ","
    << currentBestMove.second.second << std::endl;
    return ai_move_piece(currentBestMove);

}

Move ChessBot::ai_can_take_attacker(SchaakStuk* piece) {
    Game::pointerRequireNonNull(piece);
    for(SchaakStuk* attacker: game_->get_pieces_of_color(Game::opposite(piece->get_color())))
        if(Game::vector_contains_tile(piece->valid_moves(game_), piece->get_position()))
            for(int i = 1; i <= attacker->get_numeric_value(); i++)
                for(SchaakStuk* defender: game_->get_pieces_with_numeric_value(piece->get_color(), i))
                    if(Game::vector_contains_tile(defender->valid_moves(game_), attacker->get_position()))
                        return {defender, attacker->get_position()};
    return {nullptr, {}};
}

bool ChessBot::ai_resolve_check(ZW color) {
    SchaakStuk* koning = game_->find_king(color);
    Tiles threats = game_->get_tiles_to_king(koning);
    for(int i = 1; i <= 9; i++)
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i))
            for(const auto &move: piece->valid_moves(game_))
                if(Game::vector_contains_tile(threats, move) && ai_move_piece({koning, move}))
                    return true;
    throw BotCantResolveCheck(color);
}

bool ChessBot::ai_move_piece(Move move){
    Game::pointerRequireNonNull(move.first);
    if(!game_->move(move.first, move.second))
        return false;
    game_->piece_moved(scene_, move.first, move.second, Game::is_enpassant(move.first, move.second));
    return true;
}

bool ChessBot::ai_move_leads_to_mate(Move move) {
    Game::pointerRequireNonNull(move.first);
    MoveSimulation backup = {game_, move.first, move.second};
    bool checkmate = game_->checkmate(Game::opposite(move.first->get_color()));
    backup.restore();
    return checkmate;
}

bool ChessBot::ai_move_leads_to_check(Move move) {
    Game::pointerRequireNonNull(move.first);
    MoveSimulation backup = {game_, move.first, move.second};
    bool check = game_->check(Game::opposite(move.first->get_color()));
    backup.restore();
    return check;
}

int ChessBot::ai_count_tiles_after_move(Move move) {
    Game::pointerRequireNonNull(move.first);
    MoveSimulation backup = {game_, move.first, move.second};
    int count = game_->get_threatened_tiles(move.first->get_color()).size();
    backup.restore();
    return count;
}

void ChessBot::debug(const std::string& message, int depth) {
    for(int i = 0; i < depth; i++)
        std::cout << "\t";
    std::cout << message << std::endl;
}
