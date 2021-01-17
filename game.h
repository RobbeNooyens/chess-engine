//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H

#include "sharedtypes.h"
#include "config.h"
#include "promotepawn.h"
#include "ChessBot.h"

typedef std::map<std::string, std::string> JSON;

class Koning;
class Toren;
class Game {
public:
    // Constructor and destructor
    Game();
    ~Game();

    // Recycling
    void recycle();
    void recycle_board();
    void fill_board_with_nullpointers();

    // Getters
    SchaakStuk* get_piece(Tile) const;
    Pieces get_pieces_on_board() const;
    Pieces get_pieces_of_color(ZW) const;
    Pieces get_pieces_with_numeric_value(ZW, int) const;
    Tiles get_tiles_to_king(SchaakStuk*) const;
    Tiles get_threatened_tiles(ZW color);
    Tile get_enpassant_tile(ZW) const;
    bool king_moved(ZW) const;
    static bool is_enpassant(SchaakStuk*, Tile) ;

    // Setters
    void set_start_board();
    void set_piece(Tile, SchaakStuk*);
    void set_enpassant_tile(ZW, Tile);
    void set_king_moved(ZW, bool);
    void set_chessbot(ChessBoard*);

    // Game mechanics
    bool check(ZW) const;
    bool checkmate(ZW);
    bool stalemate(ZW);
    bool move_prevents_checkmate(SchaakStuk*, Tile);
    bool move(SchaakStuk*, Tile);

    // Events
    void on_tile_click(ChessBoard*, Tile);
    void piece_moved(ChessBoard*, SchaakStuk*, Tile, bool);
    void undo(ChessBoard*);
    void redo(ChessBoard*);

    // Update methods
    void update_tiles(ChessBoard*);
    void update_threatened_pieces(ChessBoard*);
    void update_options(VisualOptions);
    void update_board(ChessBoard*) const;

    // Helper methods
    static bool vector_contains_tile(const Tiles&, Tile);
    static ZW opposite(ZW);
    static void popup(std::string&);
    static void pointerRequireNonNull(void*);

    // Mapping methods
    static SchaakStuk* character_to_piece(char, Tile);
    static char piece_to_character(SchaakStuk*);
    static std::string tile_to_string(Tile);
    static Tile string_to_tile(std::string&);
    static std::string map_to_string(JSON&);
    static JSON string_to_map(std::string&);

    // Search
    Koning* find_king(ZW) const;
    std::vector<Toren*> find_rooks(ZW) const;

    // Callback
    void promote_piece_selected(PieceType, ChessBoard*, Tile);

    // Save and load
    std::string save() const;
    void load(ChessBoard*, std::string&, bool=true);

private:
    ZW turn_ = wit;
    bool whiteKingMoved_ = false;
    bool blackKingMoved_ = false;
    Tile enpassantWhite_;
    Tile enpassantBlack_;
    SchaakStuk* bord_[8][8]{};
    SchaakStuk* selectedPiece_ = nullptr;
    GameConfig config_;
    VisualOptions options_ = {false, false, false};
    PromotePawnDialog dialog_;
    std::vector<std::string> redoStack_;
    std::vector<std::string> undoStack_;
    std::string currentState;
    ChessBot bot;
    // Save and load board
    std::string save_board() const;
    void load_board(std::string&);
};


#endif //SCHAKEN_GAME_H
