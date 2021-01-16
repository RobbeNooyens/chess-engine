//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "sharedtypes.h"
#include "config.h"
#include "promotepawn.h"

typedef std::map<std::string, std::string> JSON;

class Koning;
class Toren;
class Game {
public:
    Game();
    ~Game();
    void recycle();
    void recycle_board();

    // Getters
    SchaakStuk* get_piece(Tile) const;
    Pieces get_pieces_on_board() const;
    Pieces get_pieces_of_color(ZW) const;
    Tiles get_threatened_tiles(ZW color);
    Tile get_enpassant_tile(ZW) const;

    // Setters
    void set_start_board();
    void set_piece(Tile, SchaakStuk*);
    void set_enpassant_tile(ZW, Tile);
    void set_king_moved(ZW, bool);

    // Game state checks
    bool check(ZW) const;
    bool checkmate(ZW);
    bool stalemate(ZW);
    bool move_prevents_checkmate(SchaakStuk*, Tile);
    bool king_moved(ZW) const;

    // Events
    void on_tile_click(ChessBoard*, Tile);
    void piece_moved(ChessBoard*, Tile, bool);

    // Update methods
    void update_tiles(ChessBoard*);
    void update_threatened_pieces(ChessBoard*);
    void update_options(VisualOptions);

    // Helper methods
    bool vector_contains_tile(const Tiles&, Tile) const;
    SchaakStuk* character_to_piece(char, Tile) const;
    char piece_to_character(SchaakStuk*) const;
    Koning* find_king(ZW) const;
    std::vector<Toren*> find_rooks(ZW) const;
    ZW opposite(ZW) const;
    void fill_board_with_nullpointers();
    void promote_piece_selected(PieceType, ChessBoard*, Tile);
    bool move(SchaakStuk*, Tile);
    std::string save() const;
    void load(std::string&);
    std::string saveBoard() const;
    void loadBoard(std::string&);
    std::string tile_to_string(Tile) const;
    Tile string_to_tile(std::string&) const;
    std::string map_to_string(JSON&) const;
    JSON string_to_map(std::string&) const;
private:
    // Private members
    SchaakStuk* bord_[8][8];
    ZW turn_ = wit;
    SchaakStuk* selectedPiece_ = nullptr;
    Tile enpassantWhite_;
    Tile enpassantBlack_;
    bool whiteKingMoved_ = false;
    bool blackKingMoved_ = false;
    GameConfig config_;
    PromotePawnDialog dialog_;
    VisualOptions options_ = VisualOptions(false, false, false);
};


#endif //SCHAKEN_GAME_H
