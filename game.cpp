#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
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
            this->setPiece(tile(i, j), pieceFromCharacter(initialSetup[i][j], tile(i, j)));
}

SchaakStuk* Game::pieceFromCharacter(char c, tile position) const{
    int r = position.first, k = position.second;
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

bool Game::is_valid_move(SchaakStuk* s, tile position) {
    // Checks if there's a match in the vector of possible moves
    tiles moves = s->validMoves(this);
    std::any_of(moves.begin(), moves.end(), [position](tile &move){
        return move.first == position.first && move.second == position.second;
    });
//    for(const auto &move: s->validMoves(this)){
//        if(move.first == position.first && move.second == position.second){
//            return true;
//        }
//    }
    return false;
}

void Game::onTileClick(ChessBoard* scene, tile position) {
    SchaakStuk* pieceOnTarget = getPiece(position);
    // Player selected one of his own pieces so make it the current selected piece
    if(pieceOnTarget != nullptr && selectedPieceOwner(pieceOnTarget) == turn){
        // TODO: Check for stalemate
        // Update the scene with visual information
        scene->removeAllMarking();
        scene->setTileSelect(position.first, position.second, true);
        selectedPiece = pieceOnTarget;
        updateFocusTiles(scene);
        return;
    }
    // No piece has been selected and neither did the player, so nothing can happen
    if(selectedPiece == nullptr)
        return;
    // Try to move the selected piece to the clicked tile
    if(move(selectedPiece, position)){
        // Update the scene in case the move was succesfully executed
        turn = (turn == black) ? white : black;
        scene->update();
        scene->removeAllMarking();
        selectedPiece = nullptr;
    }
}

void Game::updateFocusTiles(ChessBoard *scene) {
    tiles moves = selectedPiece->geldige_zetten(this);
    // TODO: remove pinned moves
    //    selectedPiece->removePinnedMoves(this, moves);
    // Set for every possible move the corresponding tile to a focus state
    for(const auto &move: moves)
        scene->setTileFocus(move.first, move.second, true);
}

player Game::selectedPieceOwner(const SchaakStuk* s) const { return s->getKleur() == zwart ? black : white; }

bool Game::move(SchaakStuk* s, tile position) {
    // Checks if the move is valid. If not, return false
    if(!is_valid_move(s, position))
        return false;
    // Delete de piece on the target spot from memory
    SchaakStuk* pieceOnTarget = getPiece(position);
    delete pieceOnTarget;
    // Set the current tile to a nullptr and the destination to the current piece
    setPiece(tile(s->getRow(), s->getColumn()), nullptr);
    setPiece(position, s);
    // Update the piece's member variables row and column
    s->setPosition(position);
    return true;
}

SchaakStuk* Game::findKing(ZW color) const{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            SchaakStuk* piece = getPiece(tile(i, j));
            if(piece->getKleur() == color && piece->piece().type() == piece->piece().King){
                return piece;
            }
        }
    }
    return nullptr;
}

bool Game::schaak(ZW color) const {
    SchaakStuk* king = findKing(color);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            SchaakStuk* piece = getPiece(tile(i, j));
            if(piece == nullptr || piece->getKleur() == color)
                continue;
            if(piece->canTakeAt(this, tile(king->getRow(), king->getColumn())))
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
#pragma clang diagnostic pop