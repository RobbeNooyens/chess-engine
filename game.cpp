//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {}

Game::~Game() {}

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

SchaakStuk* Game::pieceFromCharacter(char c, int r, int k){
    // Create chesspieces based on their character representation in the initialSetup 2D array
    zw color = std::islower(c) ? zwart : wit;
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

bool Game::isValidMove(SchaakStuk * s, int r, int k) {
    // Checks if there's a match in the vector of possible moves
    std::vector<std::pair<int, int>> moves = s->valid_moves(this);
    for(const auto &move: moves){
        if(move.first == r && move.second == k){
            return true;
        }
    }
    return false;
}

void Game::onTileClick(ChessBoard* scene, int r, int k) {
    SchaakStuk* pieceOnTarget = getPiece(r, k);
    if(selectedPiece == nullptr){
        if(pieceOnTarget == nullptr)
            return;
        else if(selectedPieceOwner(pieceOnTarget) == turn){
            scene->setTileSelect(r, k, true);
            selectedPiece = pieceOnTarget;
            updateFocusTiles(scene);
            scene->update();
            turn = (turn == black) ? white : black;
        }
    } else {
        move(selectedPiece, r, k);
        scene->removeAllMarking();
        selectedPiece = nullptr;
    }
}

void Game::updateFocusTiles(ChessBoard *scene) {
    std::vector<std::pair<int,int>> moves = selectedPiece->valid_moves(this);
    for(const auto &move: moves){
        if(getPiece(move.first, move.second) == nullptr)
            scene->setTileFocus(move.first, move.second, true);
        else
            scene->setPieceThreat(move.first, move.second, true);
    }
}

player Game::selectedPieceOwner(SchaakStuk* s) { return s->getKleur() == zwart ? black : white; }

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s, int r, int k) {
    if(!isValidMove(s, r, k))
        return false;
    SchaakStuk* pieceOnTarget = getPiece(r, k);
    delete pieceOnTarget;
    setPiece(s->getRow(), s->getColumn(), nullptr);
    setPiece(r, k, s);
    s->updatePosition(r, k);
    return true;
}

// Geeft true als kleur schaak staat
bool Game::schaak(zw kleur) {
    return false;
}

// Geeft true als kleur schaakmat staat
bool Game::schaakmat(zw kleur) {
    return false;
}

// Geeft true als kleur pat staat
// (pat = geen geldige zet mogelijk, maar kleur staat niet schaak;
// dit resulteert in een gelijkspel)
bool Game::pat(zw kleur) {
    return false;
}