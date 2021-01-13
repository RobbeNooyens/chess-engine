//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include <vector>

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
    SchaakStuk(ZW kleur, int row, int column): kleur(kleur), row(row), column(column) {}

    virtual Piece piece() const=0;
    virtual std::vector<std::pair<int,int>> valid_moves(const Game* game) const= 0;
    virtual SchaakStuk* copy() const = 0;

    std::vector<std::pair<int, int>> moves_from_directions(const Game* game, const std::vector<Direction>& directions) const;

    ZW getKleur() const {return kleur;}

    int getRow() const {return row;}
    int getColumn() const {return column;}
    void updatePosition(int r, int c);

    bool canMoveTo(const Game* game, int r, int c) const;
    bool canTakeAt(const Game* game, int r, int c) const;
    void removePinnedMoves(const Game*, std::vector<std::pair<int,int>>&) const;

protected:
    int row;
    int column;
    ZW kleur;
    bool isPinned(Game*, int, int);

};

class Pion:public SchaakStuk {
public:
    Pion(ZW kleur, int row, int column, PawnDirection dir): SchaakStuk(kleur, row, column), moveDirection(dir) {}

    Piece piece() const override {
        return {Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
    SchaakStuk* copy() const override { return new Pion(getKleur(), getRow(), getColumn(), moveDirection); }

private:
    PawnDirection moveDirection;
};

class Toren:public SchaakStuk {
public:
    Toren(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Rook,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
    SchaakStuk* copy() const override { return new Toren(getKleur(), getRow(), getColumn()); }
};

class Paard:public SchaakStuk {
public:
    Paard(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Knight,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
    SchaakStuk* copy() const override { return new Paard(getKleur(), getRow(), getColumn());}
};

class Loper:public SchaakStuk {
public:
    Loper(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
    SchaakStuk* copy() const override { return new Loper(getKleur(), getRow(), getColumn()); }
};

class Koning:public SchaakStuk {
public:
    Koning(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::King,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
    SchaakStuk* copy() const override { return new Koning(getKleur(), getRow(), getColumn()); }
};

class Koningin:public SchaakStuk {
public:
    Koningin(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Queen,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
    SchaakStuk* copy() const override { return new Koningin(getKleur(), getRow(), getColumn());}
};

#endif //SCHAKEN_SCHAAKSTUK_H
