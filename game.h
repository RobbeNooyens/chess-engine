//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "SchaakStuk.h"

enum player{black, white};

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(SchaakStuk*,int, int);

    // Game state checks
    bool schaak(zw);
    bool schaakmat(zw);
    bool pat(zw);
    void setStartBord();

    // Getter and setter for a piece on the board
    SchaakStuk* getPiece(int r, int k) const {return bord[r][k];}
    void setPiece(int r, int k, SchaakStuk* s) { bord[r][k] = s; }

    // Events
    void onTileClick(ChessBoard*, int, int);

private:
    // Private members
    SchaakStuk* bord[8][8];
    player turn = white;
    SchaakStuk* selectedPiece = nullptr;
    // Helper methods
    bool isValidMove(SchaakStuk*, int, int);
    void updateFocusTiles(ChessBoard* scene);
    static player selectedPieceOwner(SchaakStuk*) ;
    static SchaakStuk* pieceFromCharacter(char, int, int);
};


#endif //SCHAKEN_GAME_H
