//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include "game.h"

void SchaakStuk::updatePosition(int r, int c) {
    this->row = r;
    this->column = c;
}

bool SchaakStuk::canMoveTo(Game* game, int r, int c) const{
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    return game->getPiece(r, c) != nullptr;
}

bool SchaakStuk::canTakeAt(Game* game, int r, int c) const{
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    SchaakStuk* piece = game->getPiece(r, c);
    if(piece == nullptr)
        return false;
    return piece->getKleur() != kleur;
}

std::vector<std::pair<int,int>> Pion::valid_moves(Game* game) const {
    std::vector<std::pair<int,int>> moves;
    int dirRelative = moveDirection == up ? -1 : 1;
    int nextRow = row + dirRelative;

    // Check if move forward is possible
    if (this->canMoveTo(game, nextRow, column))
        moves.emplace_back(nextRow, column);

    // Check if pawn can take a piece sideways
    if(this->canTakeAt(game, nextRow, column-1))
        moves.emplace_back(nextRow, column-1);
    if(this->canTakeAt(game, nextRow, column+1))
        moves.emplace_back(nextRow, column+1);

    // Check if pawn can move two squares
    if(row == (moveDirection == up ? 6 : 1) && this->canMoveTo(game, nextRow + dirRelative, column))
        moves.emplace_back(nextRow + dirRelative, column);

    return moves;
}

std::vector<std::pair<int,int>> Toren::valid_moves(Game* game) const {
    std::vector<std::pair<int,int>> moves;
    return moves;
}

std::vector<std::pair<int,int>> Paard::valid_moves(Game* game) const {
    std::vector<std::pair<int,int>> moves;
    return moves;
}

std::vector<std::pair<int,int>> Loper::valid_moves(Game* game) const {
    std::vector<std::pair<int,int>> moves;
    return moves;
}

std::vector<std::pair<int,int>> Koningin::valid_moves(Game* game) const {
    std::vector<std::pair<int,int>> moves;
    return moves;
}

std::vector<std::pair<int,int>> Koning::valid_moves(Game* game) const {
    std::vector<std::pair<int,int>> moves;
    return moves;
}