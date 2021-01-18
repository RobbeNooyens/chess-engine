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
    // Check if own king is mated
    if(game_->checkmate(color))
        return true;
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
                if(piece->is_safe_move(game_, move) && ai_move_leads_to_check({piece, move}, Game::opposite(color)))
                    return ai_move_piece({piece, move});
    ChessBot::debug("It's not possible to safely check opponents king", 1);
    // Check if any high value piece is being attacked
    ChessBot::debug("Checking if any high value piece is being threatened");
    Tiles threats = game_->get_threatened_tiles(Game::opposite(color));
    Pieces threatened;
    // Add highest threatened pieces to the list
    for(int i = 9; i >= 2 && threatened.empty(); i--)
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i))
            if(Game::vector_contains_tile(threats, piece->get_position()))
                threatened.push_back(piece);
    // Resolve threatened pieces that arent covered
    if(!threatened.empty()) {
        for(SchaakStuk* piece: threatened)
            if(!ai_piece_covered(piece))
                if(ai_resolve_threatened_piece(piece))
                    return true;
    }
    ChessBot::debug("No high value piece is being threathened or can escape safely", 1);
    // Check if bot can take an enemy piece
    ChessBot::debug("Checking if bot can take one of opponents pieces");
    bool foundTarget = false;
    SchaakStuk* ownPiece = nullptr;
    Tile targetTile = {};
    int targetValue = 0;
    for(int i = 1; i <= 9; i++){
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i)){
            for(const auto &move: piece->valid_moves(game_)){
                SchaakStuk* target = game_->get_piece(move);
                if(target == nullptr)
                    continue;
                if(target->get_numeric_value() >= i && target->get_numeric_value() >= targetValue){
                    foundTarget = true;
                    ownPiece = piece;
                    targetTile = target->get_position();
                    targetValue = target->get_numeric_value();
                }
            }
        }
    }
    if(foundTarget)
        return ai_move_piece({ownPiece, targetTile});
    ChessBot::debug("Can't taken an opponents piece safely", 1);
    const int emptyTiles = ai_count_empty_start_tiles(color);
    // Rokade if possible
    if(emptyTiles >= 5){
        Koning* koning = game_->find_king(color);
        for(Toren* rook: game_->find_rooks(color)){
            std::pair<bool, Tile> rokade = koning->can_rokade(game_, rook);
            if(rokade.first)
                return ai_move_piece({koning, rokade.second});
        }
    }
    // Move random piece
    ChessBot::debug("Searching for the best random move");
    const bool canMoveQueen = ai_count_empty_start_tiles(color) >= 7;
    const int initialThreatenedSum = ai_sum_of_threatened_pieces(color);
    int attackingTiles = 0;
    Move currentBestMove;
    for(SchaakStuk* piece: game_->get_pieces_of_color(color)){
        if(piece->type() == king || piece->type() == rook || (!canMoveQueen && piece->type() == queen))
            continue;
        for(const auto &move: piece->valid_moves(game_)){
            if(!ai_safe_move({piece, move}))
                continue;
            MoveSimulation backup = {game_, piece, move};
            int tiles = game_->get_threatened_tiles(piece->get_color()).size();
            int threatenedSum = ai_sum_of_threatened_pieces(color);
            backup.restore();
            if(tiles > attackingTiles && threatenedSum <= initialThreatenedSum){
                currentBestMove = {piece, move};
                attackingTiles = tiles;
            }
        }
    }
    return ai_move_piece(currentBestMove);
}

bool ChessBot::ai_resolve_threatened_piece(SchaakStuk* piece) {
    // Check if a smaller own piece can take the attacker
    Pieces attackers = piece->get_attackers(game_);
    SchaakStuk* highestAttacker = attackers.front();
    for(SchaakStuk* attacker: attackers)
        if(attacker->get_numeric_value() > piece->get_numeric_value() && ai_can_take_attacker(piece, attacker))
            return ai_move_piece({piece, attacker->get_position()});
        else if(highestAttacker == nullptr || attacker->get_numeric_value() > highestAttacker->get_numeric_value())
            highestAttacker = attacker;
    // Check if the piece can flee to a safe field
    for(const auto &move: piece->valid_moves(game_))
        if(ai_safe_move({piece, move}))
            return ai_move_piece({piece, move});
    // Take most value attacker
    if(highestAttacker)
        return ai_move_piece({piece, highestAttacker->get_position()});
    return false;
}

bool ChessBot::ai_safe_move(Move move) const {
    if(move.first->is_safe_move(game_, move.second))
        return true;
    MoveSimulation backup = {game_, move.first, move.second};
    bool safe = ai_piece_covered(move.first);
    backup.restore();
    return safe;
}

bool ChessBot::ai_piece_covered(SchaakStuk* piece) const {
    if(!piece->is_covered(game_))
        return false;
    for(SchaakStuk* attacker: piece->get_attackers(game_))
        if(attacker->get_numeric_value() < piece->get_numeric_value())
            return false;
    return true;
}

bool ChessBot::ai_can_take_attacker(SchaakStuk* piece, SchaakStuk* attacker) const {
    Game::pointerRequireNonNull(piece);
    for(int i = 1; i <= attacker->get_numeric_value(); i++)
        for(SchaakStuk* defender: game_->get_pieces_with_numeric_value(piece->get_color(), i))
            if(Game::vector_contains_tile(defender->valid_moves(game_), attacker->get_position()))
                return true;
    return false;
}

bool ChessBot::ai_resolve_check(ZW color) {
    SchaakStuk* koning = game_->find_king(color);
    Tiles threats = game_->get_tiles_to_king(koning);
    for(int i = 1; i <= 9; i++)
        for(SchaakStuk* piece: game_->get_pieces_with_numeric_value(color, i))
            for(const auto &move: piece->valid_moves(game_))
                if(Game::vector_contains_tile(threats, move) && !ai_move_leads_to_check({piece, move}, color))
                    return ai_move_piece({piece, move});
    // Check if king can take the piece
    for(const auto &move: koning->valid_moves(game_))
        if(!ai_move_leads_to_check({koning, move}, color))
            return ai_move_piece({koning, move});
    throw BotCantResolveCheck(color);
}

bool ChessBot::ai_move_piece(Move move) const{
    Game::pointerRequireNonNull(move.first);
    if(!game_->move(move.first, move.second))
        return false;
    game_->piece_moved(scene_, move.first, move.second, Game::is_enpassant(move.first, move.second));
    return true;
}

bool ChessBot::ai_move_leads_to_mate(Move move) const{
    Game::pointerRequireNonNull(move.first);
    MoveSimulation backup = {game_, move.first, move.second};
    bool checkmate = game_->checkmate(Game::opposite(move.first->get_color()));
    backup.restore();
    return checkmate;
}

bool ChessBot::ai_move_leads_to_check(Move move, ZW color) const{
    Game::pointerRequireNonNull(move.first);
    MoveSimulation backup = {game_, move.first, move.second};
    bool check = game_->check(color);
    backup.restore();
    return check;
}

int ChessBot::ai_count_empty_start_tiles(ZW color) const{
    int count = 0;
    int startRow = color == zwart ? 0 : 6;
    for(int i = startRow; i <= startRow+1; i++)
        for(int j = 0; j < 8; j++)
            if(game_->get_piece({i, j}) == nullptr)
                count++;
    return count;
}

int ChessBot::ai_sum_of_threatened_pieces(ZW color) const {
    int sum = 0;
    for(SchaakStuk* piece: game_->get_pieces_of_color(color))
        if(!piece->get_attackers(game_).empty())
            sum += piece->get_numeric_value();
    return sum;
}

void ChessBot::debug(const std::string& message, int depth) const{
    if(!debugBot_)
        return;
    for(int i = 0; i < depth; i++)
        std::cout << ">>";
    std::cout << message << std::endl;
}
