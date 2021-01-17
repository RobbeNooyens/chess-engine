//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H

#include "sharedtypes.h"

class SchaakStuk {
public:
    // Constructor en destructor
    SchaakStuk(ZW, Tile);
    virtual ~SchaakStuk() = default;

    // Getters
    ZW get_color() const;
    int get_row() const;
    int get_column() const;
    Tile get_position() const;
    virtual Tiles get_path_to(const Game*, Tile) const = 0;
    virtual PieceType type() const = 0;
    virtual int get_numeric_value() const = 0;

    // Setters
    void set_position(Tile);

    // Verify methods
    static bool can_move_to(const Game*, Tile) ;
    bool can_take_at(const Game*, Tile) const;
    bool is_pinned(Game*, Tile);
    bool is_safe_move(Game*, Tile);

    // Helper methods
    virtual Tiles geldige_zetten(const Game*) const= 0;
    Tiles valid_moves(Game*);
    Tiles moves_from_directions(const Game*, Directions&) const;
    Tiles moves_from_positions(const Game*, Directions &) const;
    Tiles path_to_target(const Game*, Tile, Directions&) const;
    void remove_pinned_moves(Game*, Tiles&);

    // Wrappers
    virtual Piece piece() const=0;

private:
    Tile position_;
    ZW kleur_;
};

class Pion:public SchaakStuk {
public:
    Pion(ZW, Tile, PawnDirection);
    ~Pion() override = default;

    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Tiles get_threats(const Game* game);

    Piece piece() const override;
    PieceType type() const override;

    int get_numeric_value() const override;

private:
    PawnDirection moveDirection;
};

class Toren:public SchaakStuk {
public:
    Toren(ZW, Tile);
    ~Toren() override = default;

    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;

    Piece piece() const override;
    PieceType type() const override;

    int get_numeric_value() const override;

private:
    Directions directions_ = {{1,0},{-1,0},{0,1},{0,-1}};
};

class Paard:public SchaakStuk {
public:
    Paard(ZW, Tile);
    ~Paard() override = default;

    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;

    Piece piece() const override;
    PieceType type() const override;

    int get_numeric_value() const override;

private:
    Directions directions_ = {{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1}};
};

class Loper:public SchaakStuk {
public:
    Loper(ZW, Tile);
    ~Loper() override = default;

    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;

    Piece piece() const override;
    PieceType type() const override;

    int get_numeric_value() const override;

private:
    Directions directions_ = {{1,1},{1,-1},{-1,-1},{-1,1}};
};

class Koning:public SchaakStuk {
public:
    Koning(ZW, Tile);
    ~Koning() override = default;

    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;

    Piece piece() const override;
    PieceType type() const override;

    std::pair<bool, Tile> can_rokade(const Game*, const Toren*) const;
    bool safe_at(const Game*, Tile) const;

    int get_numeric_value() const override;

private:
    Directions directions_ = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
};

class Koningin:public SchaakStuk {
public:
    Koningin(ZW, Tile);
    ~Koningin() override = default;

    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;

    Piece piece() const override;
    PieceType type() const override;

    int get_numeric_value() const override;

private:
    Directions directions_ = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,-1},{-1,1}};
};

#endif //SCHAKEN_SCHAAKSTUK_H
