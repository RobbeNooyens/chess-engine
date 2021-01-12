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

    bool move(SchaakStuk* s,int r, int k); // Verplaats stuk s naar rij r en kolom k

    bool schaak(zw);
    bool schaakmat(zw);
    bool pat(zw);
    void setStartBord();

    SchaakStuk* getPiece(int r, int k) const {return bord[r][k];}
    void setPiece(int r, int k, SchaakStuk* s) { bord[r][k] = s; }

    void onTileClick(ChessBoard* scene, int r, int k);
    void updateFocusTiles(ChessBoard* scene);

private:
    SchaakStuk* bord[8][8];
    player turn = white;
    SchaakStuk* selectedPiece = nullptr;
    bool isValidMove(SchaakStuk*, int, int);
    static player selectedPieceOwner(SchaakStuk*) ;
    static SchaakStuk* pieceFromCharacter(char c, int r, int k);
};


#endif //SCHAKEN_GAME_H
