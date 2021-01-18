//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <iostream>
#include <exceptions/KingNotFoundException.h>
#include <sstream>
#include <QtWidgets/QMessageBox>
#include <exceptions/NullPointerException.h>
#include "game.h"
#include "mainwindow.h"
#include "chessboard.h"
#include "MoveSimulation.h"


// Constructor and Destructor

Game::Game():bot(ChessBot(this, nullptr)) {
    dialog_.link_game(this);
}

Game::~Game() { recycle(); }

// Recycling

void Game::recycle() {
    recycle_board();
    fill_board_with_nullpointers();
    selectedPiece_ = nullptr;
    turn_ = wit;
    enpassantWhite_ = {-1, -1};
    enpassantBlack_ = {-1, -1};
    whiteKingMoved_ = false;
    blackKingMoved_ = false;
}

void Game::recycle_board() {
    for(auto & i : bord_)
        for(auto & j : i)
            delete j;
}

void Game::fill_board_with_nullpointers() {
    for(auto & i : bord_)
        for(auto & j : i)
            j = nullptr;
}

// Getters

SchaakStuk* Game::get_piece(Tile tile) const {
    return bord_[tile.first][tile.second];
}

Tile Game::get_enpassant_tile(ZW color) const { return color == zwart ? enpassantBlack_ : enpassantWhite_;}

Tiles Game::get_threatened_tiles(ZW color) {
    Tiles tiles;
    // Union of all pieces' valid moves
    for(auto &piece: get_pieces_of_color(color)){
        Tiles moves = piece->valid_moves(this);
        // Pawn can only take sideways although it can move forward
        if(piece->type() == pawn)
            moves = ((Pion*) piece)->get_threats(this);
        // Add move to tiles if move isn't in tiles yet
        for(const auto &move: moves)
            if(!vector_contains_tile(tiles, move))
                tiles.push_back(move);
    }
    return tiles;
}

Pieces Game::get_pieces_on_board() const {
    Pieces pieces;
    for(auto &i: bord_)
        for(auto &j: i)
            if(j != nullptr)
                pieces.push_back(j);
    return pieces;
}

Pieces Game::get_pieces_of_color(ZW color) const {
    Pieces pieces = get_pieces_on_board();
    pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [color](SchaakStuk* piece){return piece->get_color() != color;}), pieces.end());
    return pieces;
}

bool Game::king_moved(ZW color) const {
    return color == zwart ? blackKingMoved_ : whiteKingMoved_;
}

Tiles Game::get_tiles_to_king(SchaakStuk* koning) const {
    Tiles threats;
    // Iterate over the opponents' pieces
    for(const auto &piece: get_pieces_of_color(opposite(koning->get_color()))){
        Tiles tiles = piece->get_path_to(this, koning->get_position());
        if(tiles.empty())
            continue;
        // Add threat positions to the threats Tiles
        for(const auto &tile: tiles)
            threats.push_back(tile);
    }
    return threats;
}

Pieces Game::get_pieces_with_numeric_value(ZW color, int value) const {
    Pieces pieces;
    // Add all pieces of the given color that have the same numeric value to the pieces vector
    for(const auto &piece: get_pieces_of_color(color))
        if(piece->get_numeric_value() == value)
            pieces.push_back(piece);
    return pieces;
}

bool Game::pawn_moved_two_rows(SchaakStuk* piece, Tile tile) {
    int rowDifference = std::abs(tile.first - piece->get_row());
    return (piece->type() == pawn && rowDifference == 2);
}

// Setters

void Game::set_piece(Tile tile, SchaakStuk*s) {
    bord_[tile.first][tile.second] = s;
}

void Game::set_enpassant_tile(ZW color, Tile tile) {
    if(color == zwart)
        enpassantBlack_ = tile;
    else
        enpassantWhite_ = tile;
}

void Game::set_king_moved(ZW color, bool moved) {
    if(color == zwart)
        blackKingMoved_ = moved;
    else
        whiteKingMoved_ = moved;
}

void Game::set_start_board() {
    // Load game state
    const BoardLayout& setup = config_.getBoardSetup();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            this->set_piece({i, j}, character_to_piece(setup[i][j], {i, j}));
    currentState = save();
    undoStack_.clear();
    redoStack_.clear();
    if(config_.enableBot && config_.botColor == wit)
        bot.ai_move(config_.botColor);
}

void Game::set_chessbot(ChessBoard* scene) {
    Game::pointerRequireNonNull(scene);
    bot = ChessBot(this, scene);
}

void Game::set_selected_piece(SchaakStuk* piece) {
    selectedPiece_ = piece;
}

// Update methods

void Game::update_options(VisualOptions options) {
    this->options_ = options;
}

void Game::update_tiles(ChessBoard *scene) {
    if(selectedPiece_ == nullptr)
        return;
    Tiles moves = selectedPiece_->valid_moves(this);
    Tiles threats = get_threatened_tiles(opposite(turn_));
    for(const auto &move: moves){
        if(options_.threats && !selectedPiece_->is_safe_move(this, move)){
            scene->setTileFocus(move.first, move.second, true);
            scene->setTileThreat(move.first, move.second, true);
        } else if(options_.moves){
            scene->setTileFocus(move.first, move.second, true);
        }
    }
}

void Game::update_threatened_pieces(ChessBoard *scene) {
    if(!options_.threatenedPieces)
        return;
    Tiles threats;
    // Visualize own threatened pieces
    if(config_.visualizeOwnThreatenedPieces) {
        threats = get_threatened_tiles(opposite(turn_));
        for (SchaakStuk* piece: get_pieces_of_color(turn_))
            if (vector_contains_tile(threats, piece->get_position()))
                scene->setPieceThreat(piece->get_row(), piece->get_column(), true);
    }
    // Visualize opponents threatened pieces
    if(config_.visualizeOpponentThreatenedPieces) {
        threats = get_threatened_tiles(turn_);
        for (SchaakStuk* piece: get_pieces_of_color(opposite(turn_)))
            if (vector_contains_tile(threats, piece->get_position()))
                scene->setPieceThreat(piece->get_row(), piece->get_column(), true);
    }
}

void Game::update_board(ChessBoard* scene) const {
    if(scene == nullptr)
        return;
    scene->clearBoard();
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            SchaakStuk* schaakStuk = get_piece(Tile(i, j));
            if(schaakStuk != nullptr)
                scene->setItem(i, j, schaakStuk->piece());
        }
    }
}

// Helper methods

ZW Game::opposite(ZW color) {
    return color == zwart ? wit : zwart;
}

bool Game::vector_contains_tile(const Tiles &moves, Tile tile) {
    // Check if position is in moves
    return std::any_of(moves.begin(), moves.end(), [tile](const Tile &move){ return move == tile; });
}

void Game::popup(std::string &text) {
    QMessageBox box;
    box.setText(QString::fromStdString(text));
    box.exec();
}

void Game::pointerRequireNonNull(void* pointer) {
    if(!pointer)
        throw NullPointerException();
}

// Mapping methods

SchaakStuk* Game::character_to_piece(char c, Tile tile) {
    // Create chesspieces based on their character representation in the initialSetup 2D array
    ZW color = std::isupper(c) ? zwart : wit;
    switch (std::tolower(c)) {
        case 'r':
            return new Toren(color, tile);
        case 'n':
            return new Paard(color, tile);
        case 'b':
            return new Loper(color, tile);
        case 'q':
            return new Koningin(color, tile);
        case 'k':
            return new Koning(color, tile);
        case 'p':
            return new Pion(color, tile, std::islower(c) ? up : down);
        default:
            return nullptr;
    }
}

char Game::piece_to_character(SchaakStuk* s) {
    char character;
    switch (s->type()) {
        case pawn:
            character = 'p';
            break;
        case rook:
            character = 'r';
            break;
        case knight:
            character = 'n';
            break;
        case bishop:
            character = 'b';
            break;
        case queen:
            character = 'q';
            break;
        case king:
            character = 'k';
            break;
        default:
            character = '.';
    }
    return s->get_color() == wit ? character : toupper(character, std::locale());
}

std::string Game::tile_to_string(Tile tile) {
    return std::to_string(tile.first) + std::to_string(tile.second);
}

Tile Game::string_to_tile(std::string &tile) {
    return (tile == "-1-1") ? Tile(-1, -1) : Tile(tile.at(0)-'0', tile.at(1)-'0');
}

std::string Game::map_to_string(JSON &map) {
    std::string output;
    for (const auto & it : map)
        output += (it.first) + ":" + it.second + ",";
    return output;
}

JSON Game::string_to_map(std::string &input) {
    std::map<std::string, std::string> output;
    std::string key;
    std::string value;
    bool writeToKey = true;
    for(const auto c: input){
        if(c == ':'){
            writeToKey = false;
            continue;
        }
        if(c == ','){
            writeToKey = true;
            output.insert({key, value});
            key.clear();
            value.clear();
            continue;
        }
        if(writeToKey)
            key.push_back(c);
        else
            value.push_back(c);
    }
    return output;
}

// Game mechanics

bool Game::check(ZW color) const {
    // Find the King of the specified color
    SchaakStuk* king = find_king(color);
    // Iterate over all pieces with the other color
    for(const auto &piece: get_pieces_of_color(opposite(color)))
        // If at least one piece can take the King, it's check
        if(vector_contains_tile(piece->geldige_zetten(this), king->get_position()))
            return true;
    // None of the enemy pieces can take the King
    return false;
}

bool Game::checkmate(ZW color) {
    // Return if none of the opponents' pieces attacks the king
    if(!check(color)) {
        return false;
    }
    SchaakStuk* koning = find_king(color);
    // Check if king can move to a safe place
    if(!koning->valid_moves(this).empty())
        return false;
    Tiles threats = get_tiles_to_king(koning);
    // Iterate over own pieces
    for(SchaakStuk* piece: get_pieces_of_color(color)) {
        if(piece->type() == king)
            continue;
        // Iterate over every own pieces' moves
        for (const auto &move: piece->valid_moves(this)) {
            // Return if the piece can't be placed in between the mating piece and the king.
            if (!std::any_of(threats.begin(), threats.end(), [move](Tile threat) { return move == threat; }))
                continue;
            // Check if the piece can block the check
            if (move_prevents_checkmate(piece, move))
                return false;
        }
    }
    return true;
}

bool Game::stalemate(ZW color) {
    // It cannot be stalemate if the king is checked
    if(check(color))
        return false;
    // Check if at least one piece can still move
    for(SchaakStuk* piece: get_pieces_of_color(color))
        if(!piece->valid_moves(this).empty())
            return false;
    return true;
}

bool Game::move(SchaakStuk* s, Tile tile) {
    // Checks if the move is valid. If not, return false
    if(!vector_contains_tile(s->valid_moves(this), tile))
        return false;
    SchaakStuk* pieceOnTarget = get_piece(tile);
    // Delete the en passant piece
    if(pieceOnTarget == nullptr && s->type() == pawn && s->get_column() != tile.second){
        Tile enpassantTile = {s->get_row(), tile.second};
        set_piece(enpassantTile, nullptr);
        delete get_piece(enpassantTile);
    }
    // Check king
    if(s->type() == king){
        set_king_moved(s->get_color(), true);
        // Check if king rokades
        if(std::abs(s->get_column() - tile.second) == 2){
            SchaakStuk* rook = get_piece({s->get_row(), s->get_column() < tile.second ? 7 : 0});
            move(rook, {s->get_row(), std::abs((s->get_column()+tile.second)/2)});
        }
    }
    // Delete piece on the target spot from memory
    delete pieceOnTarget;
    // Set the current Tile to a nullptr and the destination to the current piece
    set_piece(s->get_position(), nullptr);
    set_piece(tile, s);
    // Update the piece's member variables row_ and column_
    s->set_position(tile);
    return true;
}

bool Game::move_prevents_checkmate(SchaakStuk* piece, Tile tile) {
    MoveSimulation backup = {this, piece, tile};
    bool stillCheck = check(piece->get_color());
    backup.restore();
    return !stillCheck;
}

// Events

void Game::on_tile_click(ChessBoard* scene, Tile tile) {
    SchaakStuk* pieceOnTarget = get_piece(tile);
    // Player selected one of his own pieces so make it the current selected piece
    if(pieceOnTarget != nullptr && pieceOnTarget->get_color() == turn_){
        // Update the scene with visual information
        scene->removeAllMarking();
        scene->setTileSelect(tile.first, tile.second, true);
        selectedPiece_ = pieceOnTarget;
        update_tiles(scene);
        // Update threatened pieces
        if(options_.threatenedPieces)
            update_threatened_pieces(scene);
        return;
    }
    // No piece has been selected and neither did the Player, so nothing can happen
    if(selectedPiece_ == nullptr)
        return;
    // Check en passant
    bool enpassant = pawn_moved_two_rows(selectedPiece_, tile);
    // Try to move the selected piece to the clicked Tile
    if(move(selectedPiece_, tile))
        piece_moved(scene, selectedPiece_, tile, enpassant);
}

void Game::piece_moved(ChessBoard* scene, SchaakStuk* piece, Tile tile, bool enpassant) {
    // Check for promotion
    if(piece->type() == pawn && piece->get_row() % 7 == 0){
        dialog_.choose_promotion_piece(scene, tile);
        return;
    }
    // Update the scene in case the move was succesfully executed
    scene->removeAllMarking();
    selectedPiece_ = nullptr;
    // Check for checkmate, check and stalemate
    std::string message;
    if(config_.showMatePopup && checkmate(opposite(turn_)))
        message.append(turn_ == zwart ? "Black" : "White").append(" wins!");
    else if(config_.showCheckPopup && check(opposite(turn_)))
        message = "Check!";
    else if(config_.showMatePopup && stalemate(opposite(turn_)))
        message = "Draw!";
    // Update enpassant (-1 disables enpassant check)
    set_enpassant_tile(turn_, (enpassant ? tile : Tile(-1,-1)));
    // Switch turn
    turn_ = opposite(turn_);
    // Update threatened pieces
    if(options_.threatenedPieces)
        update_threatened_pieces(scene);
    // Push move to undo stack and clear redostack
    undoStack_.push_back(currentState);
    currentState = save();
    redoStack_.clear();
    // Update board
    update_board(scene);
    // Send state message
    if(!message.empty()){
        popup(message);
        std::cout << message << std::endl;
    }
    // Let bot do a move if it's black move
    if(config_.enableBot && turn_ == config_.botColor)
        bot.ai_move(config_.botColor);
}

void Game::undo(ChessBoard* scene) {
    if(undoStack_.empty())
        return;
    if(!currentState.empty())
        redoStack_.push_back(currentState);
    currentState = undoStack_.back();
    undoStack_.pop_back();
    load(scene, currentState, false);
    update_board(scene);
    update_tiles(scene);
    update_threatened_pieces(scene);
}

void Game::redo(ChessBoard* scene){
    if(redoStack_.empty())
        return;
    if(!currentState.empty())
        undoStack_.push_back(currentState);
    currentState = redoStack_.back();
    redoStack_.pop_back();
    load(scene, currentState, false);
    update_board(scene);
    update_tiles(scene);
    update_threatened_pieces(scene);
}

// Callback

void Game::promote_piece_selected(PieceType type, ChessBoard* scene, Tile promoteTile) {
    SchaakStuk* replacement;
    switch (type) {
        case rook:
            replacement = new Toren(turn_, promoteTile);
            break;
        case knight:
            replacement = new Paard(turn_, promoteTile);
            break;
        case bishop:
            replacement = new Loper(turn_, promoteTile);
            break;
        case queen:
        default:
            replacement = new Koningin(turn_, promoteTile);
    }
    delete get_piece(promoteTile);
    set_piece(promoteTile, replacement);
    selectedPiece_ = replacement;
    piece_moved(scene, selectedPiece_, promoteTile, false);
}

// Search

Koning* Game::find_king(ZW color) const {
    // Iterate over own pieces
    for(SchaakStuk* piece: get_pieces_of_color(color))
        if(piece->type() == king)
            return (Koning*) piece;
    // There should always be a black and a white king on the board
    throw KingNotFoundException(color);
}

std::vector<Toren*> Game::find_rooks(ZW color) const {
    std::vector<Toren*> rooks;
    // Iterate over own pieces
    for(SchaakStuk* piece: get_pieces_of_color(color))
        if(piece->type() == rook)
            rooks.push_back((Toren*) piece);
    return rooks;
}

// Save and Load

std::string Game::save_board() const {
    std::string output;
    for(const auto &row: bord_)
        for(SchaakStuk* piece: row)
            if(piece != nullptr)
                output.push_back(piece_to_character(piece));
            else
                output.push_back('.');
    return output;
}

void Game::load_board(std::string &s) {
    recycle_board();
    std::stringstream input;
    input.str(s);
    int r = 0, c = 0;
    for(const auto character: s){
        Tile tile = {r, c};
        SchaakStuk* piece = character_to_piece(character, tile);
        set_piece(tile, piece);
        c++;
        if(c == 8){
            c = 0;
            r++;
        }
        if(r == 8)
            break;
    }
}

std::string Game::save() const {
    std::map<std::string, std::string> save;
    save.insert({"board", save_board()});
    save.insert({"turn", turn_ == wit ? "white":"black"});
    save.insert({"EPW", tile_to_string(enpassantWhite_)});
    save.insert({"EPB", tile_to_string(enpassantBlack_)});
    save.insert({"WKM", whiteKingMoved_ ? "true" : "false"});
    save.insert({"BKM", blackKingMoved_ ? "true" : "false"});
    return map_to_string(save);
}

void Game::load(ChessBoard* scene, std::string &input, bool resetUndoRedoStack) {
    recycle();
    JSON data = string_to_map(input);
    if(data.count("board")){
        std::string boardData = data["board"];
        load_board(boardData);
    } else {
        set_start_board();
    }
    turn_ = data.count("turn") ? (data["turn"] == "white" ? wit : zwart) : wit;
    enpassantWhite_ = data.count("EPW") ?  string_to_tile(data["EPW"]) : Tile(-1, -1);
    enpassantBlack_ = data.count("EPB") ?  string_to_tile(data["EPB"]) : Tile(-1, -1);
    whiteKingMoved_ = data.count("WKM") != 0 && data["WKM"] == "true";
    blackKingMoved_ = data.count("BKM") != 0 && data["BKM"] == "true";
    selectedPiece_ = nullptr;
    currentState = save();
    if(resetUndoRedoStack){
        undoStack_.clear();
        redoStack_.clear();
    }
    scene->removeAllMarking();
    update_board(scene);
    update_threatened_pieces(scene);
    update_tiles(scene);
}
