//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {}

Game::~Game() {
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(bord[i][j] != nullptr)
                delete bord[i][j];
        }
    }
}

void Game::setStartBord() {
    // Load initial game state
    const char initialSetup[8][8] =
            {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
             {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
             {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            this->setPiece(i, j, pieceFromCharacter(initialSetup[i][j], i, j));
}

SchaakStuk* Game::pieceFromCharacter(char c, int r, int k) const{
    // Create chesspieces based on their character representation in the initialSetup 2D array
    ZW color = std::islower(c) ? zwart : wit;
    switch (std::tolower(c)) {
        case 'r':
            return new Toren(color, r, k);
        case 'n':
            return new Paard(color, r, k);
        case 'b':
            return new Loper(color, r, k);
        case 'q':
            return new Koningin(color, r, k);
        case 'k':
            return new Koning(color, r, k);
        case 'p':
            return new Pion(color, r, k, std::islower(c) ? up : down);
        default:
            return nullptr;
    }
}

bool Game::isValidMove(SchaakStuk * s, int r, int k) const {
    // Checks if there's a match in the vector of possible moves
    for(const auto &move: s->valid_moves(this)){
        if(move.first == r && move.second == k){
            return true;
        }
    }
    return false;
}

void Game::onTileClick(ChessBoard* scene, int r, int k) {
    SchaakStuk* pieceOnTarget = getPiece(r, k);
    // Player selected one of his own pieces so make it the current selected piece
    if(pieceOnTarget != nullptr && selectedPieceOwner(pieceOnTarget) == turn){
        // TODO: Check for stalemate
        // Update the scene with visual information
        scene->removeAllMarking();
        scene->setTileSelect(r, k, true);
        selectedPiece = pieceOnTarget;
        updateFocusTiles(scene);
        return;
    }
    // No piece has been selected and neither did the player, so nothing can happen
    if(selectedPiece == nullptr)
        return;
    // Try to move the selected piece to the clicked tile
    if(move(selectedPiece, r, k)){
        // Update the scene in case the move was succesfully executed
        turn = (turn == black) ? white : black;
        scene->update();
        scene->removeAllMarking();
        selectedPiece = nullptr;
    }
}

void Game::updateFocusTiles(ChessBoard *scene) {
    std::vector<std::pair<int,int>> moves = selectedPiece->valid_moves(this);
    // TODO: remove pinned moves
    //    selectedPiece->removePinnedMoves(this, moves);
    // Set for every possible move the corresponding tile to a focus state
    for(const auto &move: moves)
        scene->setTileFocus(move.first, move.second, true);
}

player Game::selectedPieceOwner(const SchaakStuk* s) const { return s->getKleur() == zwart ? black : white; }

bool Game::move(SchaakStuk* s, int r, int k) {
    // Checks if the move is valid. If not, return false
    if(!isValidMove(s, r, k))
        return false;
    // Delete de piece on the target spot from memory
    SchaakStuk* pieceOnTarget = getPiece(r, k);
    delete pieceOnTarget;
    // Set the current tile to a nullptr and the destination to the current piece
    setPiece(s->getRow(), s->getColumn(), nullptr);
    setPiece(r, k, s);
    // Update the piece's member variables row and column
    s->updatePosition(r, k);
    return true;
}

SchaakStuk* Game::findKing(ZW color) const{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            SchaakStuk* piece = getPiece(i, j);
            if(piece->getKleur() == color && piece->piece().type() == piece->piece().King){
                return piece;
            }
        }
    }
    return nullptr;
}

Game* Game::copy() const {
    Game* game = new Game();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(bord[i][j] != nullptr)
                game->bord[i][j] = bord[i][j]->copy();
        }
    }
    return game;
}

bool Game::schaak(ZW color) const {
    SchaakStuk* king = findKing(color);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            SchaakStuk* piece = getPiece(i, j);
            if(piece == nullptr || piece->getKleur() == color)
                continue;
            if(piece->canTakeAt(this, king->getRow(), king->getColumn()))
                return true;
        }
    }
    return false;
}

bool Game::schaakmat(ZW kleur) {
    return false;
}

bool Game::pat(ZW kleur) {
    return false;
}