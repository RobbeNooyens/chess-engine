//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <vector>

#include "SchaakStuk.h"
#include "game.h"

ZW SchaakStuk::getKleur() const {
    return kleur;
}

int SchaakStuk::getRow() const {
    return row;
}

int SchaakStuk::getColumn() const {
    return column;
}

tile SchaakStuk::getPosition() const {
    return std::make_pair(row, column);
}

void SchaakStuk::setPosition(tile position) {
    this->row = position.first;
    this->column = position.second;
}

bool SchaakStuk::canMoveTo(const Game* game, tile position) const{
    // Check if piece is off the board
    int r = position.first; int c = position.second;
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Check if the tile at the specified coordinates is empty
    return game->getPiece(r, c) == nullptr;
}

bool SchaakStuk::canTakeAt(const Game* game, tile position) const{
    // Check if piece is off the board
    int r = position.first; int c = position.second;
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Get the piece at the specified coordinates
    SchaakStuk* piece = game->getPiece(r, c);
    if(piece == nullptr)
        return false;
    return piece->getKleur() != kleur;
}

tiles SchaakStuk::moves_from_directions(const Game* game, const std::vector<Direction>& directions) const{
    tiles moves;
    // For every directions, start from current pos and follow the PawnDirection until it can no longer move further
    for(const auto direction: directions){
        // Start from the current tile with the directions being added once
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        // Add tiles until it hits another piece
        while(this->canMoveTo(game, std::make_pair(rowAbsolute, columnAbsolute))){
            moves.emplace_back(rowAbsolute, columnAbsolute);
            rowAbsolute += direction.rowRelative;
            columnAbsolute += direction.columnRelative;
        }
        // Check if the piece can take the piece it ended on
        if(this->canTakeAt(game, std::make_pair(rowAbsolute, columnAbsolute)))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

tiles SchaakStuk::moves_from_positions(const Game *game,
                                                                  const std::vector<Direction> &directions) const {
    tiles moves;
    for(const auto &direction: directions){
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        if(canMoveTo(game, std::make_pair(rowAbsolute, columnAbsolute)) || canTakeAt(game, std::make_pair(rowAbsolute, columnAbsolute)))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

bool SchaakStuk::isPinned(Game* game, int r, int c) {
    // Backup
    SchaakStuk* pieceOnTarget = game->getPiece(r, c);
    int backupRow = row;
    int backupColumn = column;
    // Simulate move
    game->setPiece(backupRow, backupColumn, nullptr);
    game->setPiece(r, c, this);
    setPosition(tile(r, c));
    bool pin = game->schaak(kleur);
    // Reset state
    game->setPiece(r, c, pieceOnTarget);
    game->setPiece(backupRow, backupColumn, this);
    setPosition(tile(backupRow, backupColumn));
    return pin;
}

void SchaakStuk::removePinnedMoves(Game* game, tiles& moves) {
    auto it = moves.begin();
    while(it != moves.end()){
        if(isPinned(game, it->first, it->second))
            it = moves.erase(it);
        else
            it++;
    }
}


// VALID_MOVES FOR EVERY PIECE
tiles Pion::geldige_zetten(const Game* game) const {
    tiles moves;
    int dirRelative = ((moveDirection == up) ? -1 : 1);
    int nextRow = row + dirRelative;

    // Check if move forward is possible
    if (this->canMoveTo(game, nextRow, column)) {
        moves.emplace_back(nextRow, column);
        // Check if pawn can move two squares
        if (row == (moveDirection == up ? 6 : 1) && this->canMoveTo(game, nextRow + dirRelative, column))
            moves.emplace_back(nextRow + dirRelative, column);
    }

    // Check if pawn can take a piece sideways
    if(this->canTakeAt(game, nextRow, column-1)) {
        moves.emplace_back(nextRow, column-1);
    }
    if(this->canTakeAt(game, nextRow, column+1)) {
        moves.emplace_back(nextRow, column+1);
    }

    removePinnedMoves(game, moves);

    return moves;
}

tiles Toren::geldige_zetten(const Game* game) const {
    // Directions: down, up, right,
    // | | |X| | |
    // | | |X| | |
    // |X|X|R|X|X|
    // | | |X| | |
    // | | |X| | |
    std::vector<Direction> directions = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1)};
    return moves_from_directions(game, directions);
}

tiles Paard::geldige_zetten(const Game* game) const {
    // Directions specific to the Knight: 2 to one axis, 1 to the other axis
    // | |X| |X| |
    // |X| | | |X|
    // | | |N| | |
    // |X| | | |X|
    // | |X| |X| |
    tiles moves;
    std::vector<Direction> directions = {Direction(-2, 1), Direction(-1, 2), Direction(1, 2), Direction(2, 1),
                                         Direction(2, -1), Direction(1,-2), Direction(-1, -2), Direction(-2, -1),};
    for(const auto &direction: directions){
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        if(canMoveTo(game, rowAbsolute, columnAbsolute) || canTakeAt(game, rowAbsolute, columnAbsolute))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

tiles Loper::geldige_zetten(const Game* game) const {
    // Directions: down-right, down-left, up-left, up-right
    // |X| | | |X|
    // | |X| |X| |
    // | | |B| | |
    // | |X| |X| |
    // |X| | | |X|
    std::vector<Direction> directions = {Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
    return moves_from_directions(game, directions);
}

tiles Koningin::geldige_zetten(const Game* game) const {
    // Directions: down, up, right, left, down-right, down-left, up-left, up-right
    // |X| |X| |X|
    // | |X|X|X| |
    // |X|X|Q|X|X|
    // | |X|X|X| |
    // |X| |X| |X|
    std::vector<Direction> directions = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1), Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
    return moves_from_directions(game, directions);
}

tiles Koning::geldige_zetten(const Game* game) const {
    // Directions: Square around the current tile
    // | | | | | |
    // | |X|X|X| |
    // | |X|K|X| |
    // | |X|X|X| |
    // | | | | | |
    tiles moves;
    std::vector<Direction> directions = {Direction(0, 1), Direction(1, 1), Direction(1, 0), Direction(1, -1),
                                         Direction(0, -1), Direction(-1,-1), Direction(-1, 0), Direction(-1, 1)};

    return moves;
}