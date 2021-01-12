//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include <vector>

enum zw{zwart,wit};
enum direction{up, down};

struct Direction {
    int rowRelative;
    int columnRelative;
    Direction(int r, int c) : rowRelative(r), columnRelative(c){}
};

class Game;
class SchaakStuk {
public:
    SchaakStuk(zw kleur, int row, int column): kleur(kleur), row(row), column(column) {}

    virtual Piece piece() const=0;
    virtual std::vector<std::pair<int,int>> valid_moves(const Game* game) const= 0;

    std::vector<std::pair<int, int>> moves_from_directions(const Game* game, const std::vector<Direction>& directions) const;

    zw getKleur() const {return kleur;}

    int getRow() const {return row;}
    int getColumn() const {return column;}
    void updatePosition(int r, int c);

    bool canMoveTo(const Game* game, int r, int c) const;
    bool canTakeAt(const Game* game, int r, int c) const;

protected:
    int row;
    int column;

private:
    zw kleur;
};

class Pion:public SchaakStuk {
public:
    Pion(zw kleur, int row, int column, direction dir): SchaakStuk(kleur, row, column), moveDirection(dir) {}

    Piece piece() const override {
        return {Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;

private:
    direction moveDirection;
};

class Toren:public SchaakStuk {
public:
    Toren(zw kleur, int row, int column):SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Rook,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
};

class Paard:public SchaakStuk {
public:
    Paard(zw kleur, int row, int column):SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Knight,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
};

class Loper:public SchaakStuk {
public:
    Loper(zw kleur, int row, int column):SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
};

class Koning:public SchaakStuk {
public:
    Koning(zw kleur, int row, int column):SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::King,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
};

class Koningin:public SchaakStuk {
public:
    Koningin(zw kleur, int row, int column):SchaakStuk(kleur, row, column) {}

    Piece piece() const override {
        return {Piece::Queen,getKleur()==wit?Piece::White:Piece::Black};
    }

    std::vector<std::pair<int,int>> valid_moves(const Game* game) const override;
};

#endif //SCHAKEN_SCHAAKSTUK_H
