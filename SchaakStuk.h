//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include <vector>

typedef std::vector<std::pair<int,int>> Tiles;
typedef std::pair<int,int> Tile;

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
    ZW get_color() const;
    int get_row() const;
    int get_column() const;
    Tile get_position() const;

    // Setters
    void set_position(Tile);

    // Verify methods
    bool can_move_to(const Game*, Tile) const;
    bool can_take_at(const Game*, Tile) const;

    // Helper methods
    void remove_pinned_moves(Game*, Tiles&);
    Tiles valid_moves(Game*);
    Tiles moves_from_directions(const Game* game, const std::vector<Direction>& directions) const;
    Tiles moves_from_positions(const Game* game, const std::vector<Direction>& directions) const;
    virtual Tiles geldige_zetten(const Game*) const= 0;

    // Wrappers
    virtual Piece piece() const=0;
protected:
    bool is_pinned(Game*, Tile);
private:
    int row;
    int column;
    ZW kleur;

};

class Pion:public SchaakStuk {
public:
    Pion(ZW kleur, int row, int column, PawnDirection dir): SchaakStuk(kleur, row, column), moveDirection(dir) {}
    Tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Pawn, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    PawnDirection moveDirection;
};

class Toren:public SchaakStuk {
public:
    Toren(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Rook, get_color() == wit ? Piece::White : Piece::Black};
    }
};

class Paard:public SchaakStuk {
public:
    Paard(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Knight, get_color() == wit ? Piece::White : Piece::Black};
    }
};

class Loper:public SchaakStuk {
public:
    Loper(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Bishop, get_color() == wit ? Piece::White : Piece::Black};
    }
};

class Koning:public SchaakStuk {
public:
    Koning(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::King, get_color() == wit ? Piece::White : Piece::Black};
    }
};

class Koningin:public SchaakStuk {
public:
    Koningin(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Piece piece() const override {
        return {Piece::Queen, get_color() == wit ? Piece::White : Piece::Black};
    }
};

#endif //SCHAKEN_SCHAAKSTUK_H
