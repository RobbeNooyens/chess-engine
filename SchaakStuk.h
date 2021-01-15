//  Student: Robbe Nooyens
//  Rolnummer: 20201010
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
    static Direction relative_direction(Tile start, Tile dest){
        int row = start.first > dest.first ? -1 : (start.first < dest.first ? 1 : 0);
        int column = start.second > dest.second ? -1 : (start.second < dest.second ? 1 : 0);
        return {row, column};
    }
    bool operator==(const Direction& other) const {
        return rowRelative == other.rowRelative && columnRelative == other.columnRelative;
    }
};

class Game;
class SchaakStuk {
public:
    // Constructor
    SchaakStuk(ZW kleur, int row, int column): kleur_(kleur), row_(row), column_(column) {}

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

    // Helper methods
    void remove_pinned_moves(Game*, Tiles&);
    Tiles valid_moves(Game*);
    Tiles moves_from_directions(const Game* game, const std::vector<Direction>& directions) const;
    Tiles moves_from_positions(const Game* game, const std::vector<Direction>& directions) const;
    Tiles path_to_target(const Game*, Tile, const std::vector<Direction>&) const;
    virtual Tiles geldige_zetten(const Game*) const= 0;

    // Wrappers
    virtual Piece piece() const=0;
private:
    int row_;
    int column_;
    ZW kleur_;

};

class Pion:public SchaakStuk {
public:
    Pion(ZW kleur, int row, int column, PawnDirection dir): SchaakStuk(kleur, row, column), moveDirection(dir) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
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
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Rook, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(1,0), Direction(-1,0), Direction(0,1), Direction(0,-1)};
};

class Paard:public SchaakStuk {
public:
    Paard(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Knight, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(-2, 1), Direction(-1, 2), Direction(1, 2), Direction(2, 1),
                                         Direction(2, -1), Direction(1,-2), Direction(-1, -2), Direction(-2, -1),};
};

class Loper:public SchaakStuk {
public:
    Loper(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
    Tiles geldige_zetten(const Game*) const override;
    Tiles get_path_to(const Game*, Tile) const override;
    Piece piece() const override {
        return {Piece::Bishop, get_color() == wit ? Piece::White : Piece::Black};
    }
private:
    const std::vector<Direction> directions_ = {Direction(1,1), Direction(1,-1), Direction(-1,-1), Direction(-1,1)};
};

class Koning:public SchaakStuk {
public:
    Koning(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
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

class Koningin:public SchaakStuk {
public:
    Koningin(ZW kleur, int row, int column): SchaakStuk(kleur, row, column) {}
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
