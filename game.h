//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "sharedtypes.h"

class Game {
public:
    Game();
    ~Game();

    // Getters
    SchaakStuk* get_piece(Tile) const;
    Pieces get_pieces_on_board() const;
    Pieces get_pieces_of_color(ZW) const;
    Tiles get_threatening_tiles(ZW);
    Tile get_enpassant_tile(ZW) const;

    // Setters
    void set_start_board();
    void set_piece(Tile, SchaakStuk*);
    void set_enpassant_tile(ZW, Tile);

    // Game state checks
    bool check(ZW) const;
    bool checkmate(ZW);
    bool stalemate(ZW color);
    bool move_prevents_checkmate(SchaakStuk*, Tile);

    // Events
    void on_tile_click(ChessBoard*, Tile, VisualOptions);
    bool move(SchaakStuk*, Tile);

    // Update methods
    void update_tiles(ChessBoard*, VisualOptions);
    void update_threatened_pieces(ChessBoard*, VisualOptions);

    // Helper methods
    bool vector_contains_tile(const Tiles&, Tile) const;
    SchaakStuk* piece_from_character(char, Tile) const;
    SchaakStuk* find_king(ZW) const;
    ZW opposite(ZW) const;
private:
    // Private members
    SchaakStuk* bord_[8][8];
    ZW turn_ = wit;
    SchaakStuk* selectedPiece_ = nullptr;
    Tile enpassantWhite;
    Tile enpassantBlack;
};


#endif //SCHAKEN_GAME_H
