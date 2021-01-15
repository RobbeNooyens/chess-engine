//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "sharedtypes.h"


typedef char BoardLayout[8][8];
class Game {
public:
    Game();
    ~Game();

    // Getters
    schaakstuk* get_piece(Tile) const;
    Pieces get_pieces_on_board() const;
    Pieces get_pieces_of_color(ZW) const;
    Tiles get_threatening_tiles(ZW);

    // Setters
    void set_start_board();
    void set_piece(Tile, schaakstuk*);

    // Game state checks
    bool check(ZW) const;
    bool checkmate(ZW);
    bool stalemate(ZW color);
    bool move_prevents_checkmate(schaakstuk*, Tile);

    // Events
    void on_tile_click(ChessBoard*, Tile, VisualOptions);
    bool move(schaakstuk*, Tile);

    // Update methods
    void update_tiles(ChessBoard*, VisualOptions);
    void update_threatened_pieces(ChessBoard*, VisualOptions);

    // Helper methods
    bool vector_contains_tile(const Tiles&, Tile) const;
    schaakstuk* piece_from_character(char, Tile) const;
    schaakstuk* find_king(ZW) const;
    ZW opposite(ZW) const;
private:
    // Private members
    schaakstuk* bord_[8][8];
    ZW turn_ = wit;
    schaakstuk* selectedPiece_ = nullptr;
};


#endif //SCHAKEN_GAME_H
