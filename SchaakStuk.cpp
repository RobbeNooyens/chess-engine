#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <vector>

#include "SchaakStuk.h"
#include "game.h"

ZW SchaakStuk::get_color() const {
    return kleur;
}

int SchaakStuk::get_row() const {
    return row;
}

int SchaakStuk::get_column() const {
    return column;
}

Tile SchaakStuk::get_position() const {
    return {row, column};
}

void SchaakStuk::set_position(Tile position) {
    this->row = position.first;
    this->column = position.second;
}

bool SchaakStuk::can_move_to(const Game* game, Tile position) const{
    // Check if piece is off the board
    int r = position.first; int c = position.second;
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Check if the Tile at the specified coordinates is empty
    return game->get_piece(position) == nullptr;
}

bool SchaakStuk::can_take_at(const Game* game, Tile position) const{
    // Check if piece is off the board
    int r = position.first; int c = position.second;
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Get the piece at the specified coordinates
    SchaakStuk* piece = game->get_piece(position);
    if(piece == nullptr)
        return false;
    return piece->get_color() != kleur;
}

Tiles SchaakStuk::moves_from_directions(const Game* game, const std::vector<Direction>& directions) const{
    Tiles moves;
    // For every directions, start from current pos and follow the PawnDirection until it can no longer move further
    for(const auto direction: directions){
        // Start from the current Tile with the directions being added once
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        // Add Tiles until it hits another piece
        while(this->can_move_to(game, Tile(rowAbsolute, columnAbsolute))){
            moves.emplace_back(rowAbsolute, columnAbsolute);
            rowAbsolute += direction.rowRelative;
            columnAbsolute += direction.columnRelative;
        }
        // Check if the piece can take the piece it ended on
        if(this->can_take_at(game, Tile(rowAbsolute, columnAbsolute)))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

Tiles SchaakStuk::moves_from_positions(const Game *game, const std::vector<Direction> &directions) const {
    Tiles moves;
    for(const auto &direction: directions){
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        if(can_move_to(game, Tile(rowAbsolute, columnAbsolute)) || can_take_at(game, Tile(rowAbsolute, columnAbsolute)))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

bool SchaakStuk::is_pinned(Game* game, Tile position) {
    // Backup
    SchaakStuk* pieceOnTarget = game->get_piece(position);
    Tile initPosition = position;
    // Simulate move
    game->set_piece(initPosition, nullptr);
    game->set_piece(position, this);
    set_position(position);
    bool pin = game->check(kleur);
    // Reset state
    game->set_piece(position, pieceOnTarget);
    game->set_piece(initPosition, this);
    set_position(initPosition);
    return pin;
}

void SchaakStuk::remove_pinned_moves(Game* game, Tiles& moves) {
    auto it = moves.begin();
    while(it != moves.end()){
        if(is_pinned(game, Tile(it->first, it->second)))
            it = moves.erase(it);
        else
            it++;
    }
}

Tiles SchaakStuk::valid_moves(Game* game) {
    Tiles moves = this->geldige_zetten(game);
    remove_pinned_moves(game, moves);
    return moves;
}

// RETURNS NAIVE POSSIBLE MOVES FOR EVERY PIECE
Tiles Pion::geldige_zetten(const Game* game) const {
    int row = this->get_row(), column = this->get_column();
    Tiles moves;
    int dirRelative = ((moveDirection == up) ? -1 : 1);
    int nextRow = row + dirRelative;

    // Check if move forward is possible
    if (this->can_move_to(game, Tile(row, column))) {
        moves.emplace_back(nextRow, column);
        // Check if pawn can move two squares
        if (row == (moveDirection == up ? 6 : 1) && this->can_move_to(game, Tile(nextRow + dirRelative, column)))
            moves.emplace_back(nextRow + dirRelative, column);
    }

    // Check if pawn can take a piece sideways
    if(this->can_take_at(game, Tile(nextRow, column - 1)))
        moves.emplace_back(nextRow, column-1);
    if(this->can_take_at(game, Tile(nextRow, column + 1)))
        moves.emplace_back(nextRow, column+1);

    return moves;
}
Tiles Toren::geldige_zetten(const Game* game) const {
    // Directions: down, up, right,
    // | | |X| | |
    // | | |X| | |
    // |X|X|R|X|X|
    // | | |X| | |
    // | | |X| | |
    std::vector<Direction> directions = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1)};
    return moves_from_directions(game, directions);
}
Tiles Paard::geldige_zetten(const Game* game) const {
    // Directions specific to the Knight: 2 to one axis, 1 to the other axis
    // | |X| |X| |
    // |X| | | |X|
    // | | |N| | |
    // |X| | | |X|
    // | |X| |X| |
    Tiles moves;
    std::vector<Direction> directions = {Direction(-2, 1), Direction(-1, 2), Direction(1, 2), Direction(2, 1),
                                         Direction(2, -1), Direction(1,-2), Direction(-1, -2), Direction(-2, -1),};
    return moves_from_positions(game, directions);
}
Tiles Loper::geldige_zetten(const Game* game) const {
    // Directions: down-right, down-left, up-left, up-right
    // |X| | | |X|
    // | |X| |X| |
    // | | |B| | |
    // | |X| |X| |
    // |X| | | |X|
    std::vector<Direction> directions = {Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
    return moves_from_directions(game, directions);
}
Tiles Koningin::geldige_zetten(const Game* game) const {
    // Directions: down, up, right, left, down-right, down-left, up-left, up-right
    // |X| |X| |X|
    // | |X|X|X| |
    // |X|X|Q|X|X|
    // | |X|X|X| |
    // |X| |X| |X|
    std::vector<Direction> directions = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1), Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
    return moves_from_directions(game, directions);
}
Tiles Koning::geldige_zetten(const Game* game) const {
    // Directions: Square around the current Tile
    // | | | | | |
    // | |X|X|X| |
    // | |X|K|X| |
    // | |X|X|X| |
    // | | | | | |
    Tiles moves;
    std::vector<Direction> directions = {Direction(0, 1), Direction(1, 1), Direction(1, 0), Direction(1, -1),
                                         Direction(0, -1), Direction(-1,-1), Direction(-1, 0), Direction(-1, 1)};
    return moves_from_positions(game, directions);
}
#pragma clang diagnostic pop