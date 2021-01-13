//
// Created by robbe on 13-1-2021.
//

#ifndef SCHAKEN_TILE_H
#define SCHAKEN_TILE_H

enum TileState{unmarked, selected, focused, threathened};

class SchaakStuk;
class Tile {
public:
    Tile(int,int);
    SchaakStuk* getPiece() const;
    SchaakStuk* popPiece();
    SchaakStuk* replacePiece(SchaakStuk*);
    int getRow() const;
    int getColumn() const;
    bool hasPiece() const;
    bool isSelected() const;
    bool isFocused() const;
    bool isThreatened() const;
    void setPiece(SchaakStuk*);
    void select();
    void focus();
    void threatened();
    void unmark();
    void removePiece();
private:
    const int row;
    const int column;
    SchaakStuk* piece;
    TileState tileState;
};


#endif //SCHAKEN_TILE_H
