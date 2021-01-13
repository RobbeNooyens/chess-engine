//
// Created by robbe on 13-1-2021.
//

#include "Tile.h"

Tile::Tile(int row, int column): row(row), column(column) {}
SchaakStuk* Tile::getPiece() const {
    return piece;
}
SchaakStuk* Tile::popPiece() {
    SchaakStuk* p = piece;
    piece = nullptr;
    return p;
}
bool Tile::hasPiece() const {
    return piece != nullptr;
}
bool Tile::isFocused() const {
    return tileState == focused;
}
bool Tile::isSelected() const {
    return tileState == selected;
}
bool Tile::isThreatened() const {
    return tileState == threathened;
}
int Tile::getRow() const {
    return row;
}
int Tile::getColumn() const {
    return column;
}
void Tile::select() {
    this->tileState = selected;
}
void Tile::focus() {
    this->tileState = focused;
}
void Tile::threatened() {
    this->tileState = threathened;
}
void Tile::setPiece(SchaakStuk * p) {
    this->piece = p;
}
void Tile::removePiece() {
    this->piece = nullptr;
}
SchaakStuk* Tile::replacePiece(SchaakStuk * p) {
    SchaakStuk* oldPiece = this->piece;
    this->piece = p;
    return oldPiece;
}