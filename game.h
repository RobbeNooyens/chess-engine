//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "SchaakStuk.h"

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(SchaakStuk* s,int r, int k); // Verplaats stuk s naar rij r en kolom k

    bool schaak(zw kleur);
    bool schaakmat(zw kleur);
    bool pat(zw kleur);
    void setStartBord();

    SchaakStuk* getPiece(int r, int k) {return bord[r][k];}
    void setPiece(int r, int k, SchaakStuk* s) { bord[r][k] = s; }

private:
    SchaakStuk* bord[8][8];

    static SchaakStuk* pieceFromCharacter(char);
};


#endif //SCHAKEN_GAME_H
