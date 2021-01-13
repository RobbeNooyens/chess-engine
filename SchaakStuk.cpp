//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <vector>

#include "SchaakStuk.h"
#include "game.h"

void SchaakStuk::updatePosition(int r, int c) {
    this->row = r;
    this->column = c;
}

bool SchaakStuk::canMoveTo(const Game* game, int r, int c) const{
    // Check if piece is off the board
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Check if the tile at the specified coordinates is empty
    return game->getPiece(r, c) == nullptr;
}

bool SchaakStuk::canTakeAt(const Game* game, int r, int c) const{
    // Check if piece is off the board
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Get the piece at the specified coordinates
    SchaakStuk* piece = game->getPiece(r, c);
    if(piece == nullptr)
        return false;
    return piece->getKleur() != kleur;
}

std::vector<std::pair<int, int>> SchaakStuk::moves_from_directions(const Game* game, const std::vector<Direction>& directions) const{
    std::vector<std::pair<int,int>> moves;
    // For every directions, start from current pos and follow the PawnDirection until it can no longer move further
    for(const auto direction: directions){
        // Start from the current tile with the directions being added once
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        // Add tiles until it hits another piece
        while(this->canMoveTo(game, rowAbsolute, columnAbsolute)){
            moves.emplace_back(rowAbsolute, columnAbsolute);
            rowAbsolute += direction.rowRelative;
            columnAbsolute += direction.columnRelative;
        }
        // Check if the piece can take the piece it ended on
        if(this->canTakeAt(game, rowAbsolute, columnAbsolute))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    removePinnedMoves(game, moves);
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
    updatePosition(r, c);
    bool pin = game->schaak(kleur);
    // Reset state
    game->setPiece(r, c, pieceOnTarget);
    game->setPiece(backupRow, backupColumn, this);
    updatePosition(backupRow, backupColumn);
    return pin;
}

void SchaakStuk::removePinnedMoves(const Game* game, std::vector<std::pair<int,int>>& moves) const {
    Game* moveSimulation = game->copy();
    SchaakStuk* pieceSimulation = moveSimulation->getPiece(row, column);
    auto it = moves.begin();
    while(it != moves.end()){
        if(pieceSimulation->isPinned(moveSimulation, it->first, it->second))
            it = moves.erase(it);
        else
            it++;
    }
    delete moveSimulation;
}


// VALID_MOVES FOR EVERY PIECE
std::vector<std::pair<int,int>> Pion::valid_moves(const Game* game) const {
    std::vector<std::pair<int,int>> moves;
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

std::vector<std::pair<int,int>> Toren::valid_moves(const Game* game) const {
    // Directions: down, up, right,
    // | | |X| | |
    // | | |X| | |
    // |X|X|R|X|X|
    // | | |X| | |
    // | | |X| | |
    std::vector<Direction> directions = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1)};
    return moves_from_directions(game, directions);
}

std::vector<std::pair<int,int>> Paard::valid_moves(const Game* game) const {
    // Directions specific to the Knight: 2 to one axis, 1 to the other axis
    // | |X| |X| |
    // |X| | | |X|
    // | | |N| | |
    // |X| | | |X|
    // | |X| |X| |
    std::vector<std::pair<int,int>> moves;
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

std::vector<std::pair<int,int>> Loper::valid_moves(const Game* game) const {
    // Directions: down-right, down-left, up-left, up-right
    // |X| | | |X|
    // | |X| |X| |
    // | | |B| | |
    // | |X| |X| |
    // |X| | | |X|
    std::vector<Direction> directions = {Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
    return moves_from_directions(game, directions);
}

std::vector<std::pair<int,int>> Koningin::valid_moves(const Game* game) const {
    // Directions: down, up, right, left, down-right, down-left, up-left, up-right
    // |X| |X| |X|
    // | |X|X|X| |
    // |X|X|Q|X|X|
    // | |X|X|X| |
    // |X| |X| |X|
    std::vector<Direction> directions = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1), Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
    return moves_from_directions(game, directions);
}

std::vector<std::pair<int,int>> Koning::valid_moves(const Game* game) const {
    // Directions: Square around the current tile
    // | | | | | |
    // | |X|X|X| |
    // | |X|K|X| |
    // | |X|X|X| |
    // | | | | | |
    std::vector<std::pair<int,int>> moves;
    std::vector<Direction> directions = {Direction(0, 1), Direction(1, 1), Direction(1, 0), Direction(1, -1),
                                         Direction(0, -1), Direction(-1,-1), Direction(-1, 0), Direction(-1, 1)};
    for(const auto &direction: directions){
        int rowAbsolute = row + direction.rowRelative;
        int columnAbsolute = column + direction.columnRelative;
        if(canMoveTo(game, rowAbsolute, columnAbsolute) || canTakeAt(game, rowAbsolute, columnAbsolute))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}