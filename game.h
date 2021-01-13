//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "SchaakStuk.h"

typedef std::vector<std::pair<int,int>> tiles;
typedef std::pair<int,int> tile;

enum player{black, white};

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(SchaakStuk*, tile);

    Game* copy() const;

    // Game state checks
    bool schaak(ZW) const;
    bool schaakmat(ZW);
    bool pat(ZW);
    void setStartBord();

    // Getter and setter for a piece on the board
    SchaakStuk* getPiece(tile) const {return bord[r][k];}
    void setPiece(tile, SchaakStuk* s) { bord[r][k] = s; }

    // Events
    void onTileClick(ChessBoard*, tile);

private:
    // Private members
    SchaakStuk* bord[8][8];
    player turn = white;
    SchaakStuk* selectedPiece = nullptr;
    // Helper methods
    bool is_valid_move(SchaakStuk*, tile);
    void updateFocusTiles(ChessBoard* scene);
    player selectedPieceOwner(const SchaakStuk*) const;
    SchaakStuk* pieceFromCharacter(char, tile) const;
    SchaakStuk* findKing(ZW color) const;
};


#endif //SCHAKEN_GAME_H
