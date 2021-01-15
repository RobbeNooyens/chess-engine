//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include "sharedtypes.h"

class Game;
class schaakstuk {
public:
    // Constructor
    schaakstuk(ZW kleur, int row, int column): kleur_(kleur), row_(row), column_(column) {}

    // Getters
    ZW get_color() const;
    int get_row() const;
    int get_column() const;
    Tile get_position() const;
    virtual Tiles get_path_to(const Game*, Tile) const = 0;

    // Setters
    void set_position(Tile);

    // Verify methods
    bool can_move_to(const Game*, Tile) const;
    bool can_take_at(const Game*, Tile) const;
    bool is_pinned(Game*, Tile);
    bool is_safe_move(Game*, Tile);

    // Helper methods
    void remove_pinned_moves(Game*, Tiles&);
    Tiles valid_moves(Game*);
    Tiles moves_from_directions(const Game*, const std::vector<Direction>&) const;
    Tiles moves_from_positions(const Game*, const std::vector<Direction>&) const;
    Tiles path_to_target(const Game*, Tile, const std::vector<Direction>&) const;
    virtual Tiles geldige_zetten(const Game*) const= 0;

    // Wrappers
    virtual Piece piece() const=0;
private:
    int row_;
    int column_;
    ZW kleur_;

};

class Pion:public schaakstuk {
public:
    Pion(ZW kleur, int row, int column, PawnDirection dir): schaakstuk(kleur, row, column), moveDirection(dir) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Tiles get_threats(const Game* game);
    Piece piece() const override {
        return {Piece::Pawn, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    PawnDirection moveDirection;
};

class Toren:public schaakstuk {
public:
    Toren(ZW kleur, int row, int column): schaakstuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Rook, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1)};
};

class Paard:public schaakstuk {
public:
    Paard(ZW kleur, int row, int column): schaakstuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Knight, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(-2, 1), Direction(-1, 2), Direction(1, 2), Direction(2, 1),
                                         Direction(2, -1), Direction(1,-2), Direction(-1, -2), Direction(-2, -1),};
};

class Loper:public schaakstuk {
public:
    Loper(ZW kleur, int row, int column): schaakstuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Bishop, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
};

class Koning:public schaakstuk {
public:
    Koning(ZW kleur, int row, int column): schaakstuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::King, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(0, 1), Direction(1, 1), Direction(1, 0),
                                               Direction(1, -1), Direction(0, -1), Direction(-1,-1),
                                               Direction(-1, 0), Direction(-1, 1)};
};

class Koningin:public schaakstuk {
public:
    Koningin(ZW kleur, int row, int column): schaakstuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Queen, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(1,0), Direction(-1,0), Direction(0,1),
                                                Direction(0,-1), Direction(1,1), Direction(1,-1),
                                                Direction(-1,-1), Direction(-1,1)};
};

#endif //SCHAKEN_SCHAAKSTUK_H
