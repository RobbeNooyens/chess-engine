//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include <vector>

typedef std::vector<std::pair<int,int>> tiles;
typedef std::pair<int,int> tile;

enum ZW{zwart,wit};
enum PawnDirection{up, down};

struct Direction {
    int rowRelative;
    int columnRelative;
    Direction(int r, int c) : rowRelative(r), columnRelative(c){}
};

class Game;
class SchaakStuk {
public:
    // Constructor
    SchaakStuk(ZW kleur, int row, int column): kleur(kleur), row(row), column(column) {}
    // Getters
    ZW getKleur() const;
    int getRow() const;
    int getColumn() const;
    tile getPosition() const;
    // Setters
    void setPosition(tile);
    // Verify methods
    bool canMoveTo(const Game*, tile) const;
    bool canTakeAt(const Game*, tile) const;
    // Helper methods
    void removePinnedMoves(Game*, tiles&);
    tiles validMoves(Game*);
    tiles moves_from_directions(const Game* game, const std::vector<Direction>& directions) const;
    tiles moves_from_positions(const Game* game, const std::vector<Direction>& directions) const;
    virtual tiles geldige_zetten(const Game*) const= 0;
    // Wrappers
    virtual Piece piece() const=0;
protected:
    bool isPinned(Game*, int, int);
private:
    int row;
    int column;
    ZW kleur;

};

class Pion:public SchaakStuk {
public:
    Pion(ZW kleur, int row, int column, PawnDirection dir): SchaakStuk(kleur, row, column), moveDirection(dir) {}
    tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black};
    }
private:
    PawnDirection moveDirection;
};

class Toren:public SchaakStuk {
public:
    Toren(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Rook,getKleur()==wit?Piece::White:Piece::Black};
    }
};

class Paard:public SchaakStuk {
public:
    Paard(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Knight,getKleur()==wit?Piece::White:Piece::Black};
    }
};

class Loper:public SchaakStuk {
public:
    Loper(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black};
    }
};

class Koning:public SchaakStuk {
public:
    Koning(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::King,getKleur()==wit?Piece::White:Piece::Black};
    }
};

class Koningin:public SchaakStuk {
public:
    Koningin(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Queen,getKleur()==wit?Piece::White:Piece::Black};
    }
};

#endif //SCHAKEN_SCHAAKSTUK_H
