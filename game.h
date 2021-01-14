//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "SchaakStuk.h"

typedef std::vector<std::pair<int,int>> Tiles;
typedef std::pair<int,int> Tile;
typedef std::vector<SchaakStuk*> Pieces;

enum player{black, white};

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    // Getters
    SchaakStuk* get_piece(Tile) const;
    Pieces get_pieces_on_board() const;
    Pieces get_pieces_of_color(ZW) const;

    // Setters
    void set_start_board();
    void set_piece(Tile, SchaakStuk*);

    // Game state checks
    bool check(ZW) const;
    bool checkmate(ZW);
    bool draw(ZW);

    // Events
    void on_tile_click(ChessBoard*, Tile);
    bool move(SchaakStuk*, Tile);

private:
    // Private members
    SchaakStuk* bord_[8][8];
    player turn_ = white;
    SchaakStuk* selectedPiece_ = nullptr;
    // Helper methods
    bool is_valid_move(Tile, const Tiles&) const;
    void update_tiles(ChessBoard* scene);
    player selected_piece_owner(const SchaakStuk*) const;
    SchaakStuk* piece_from_character(char, Tile) const;
    SchaakStuk* find_king(ZW color) const;
};


#endif //SCHAKEN_GAME_H
