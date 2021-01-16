#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <iostream>
#include <exceptions/KingNotFoundException.h>
#include "game.h"
#include "config.h"
#include "mainwindow.h"
#include "chessboard.h"


Game::Game() {}

Game::~Game() { recycle(); }

void Game::recycle() {
    selectedPiece_ = nullptr;
    for(auto &i: bord_)
        for(auto &j: i)
            delete j;
    turn_ = wit;
    enpassantWhite = {-1, -1};
    enpassantBlack = {-1, -1};
}

SchaakStuk* Game::get_piece(Tile position) const {
    return bord_[position.first][position.second];
}

void Game::set_piece(Tile position, SchaakStuk*s) {
    bord_[position.first][position.second] = s;
}

Tile Game::get_enpassant_tile(ZW color) const { return color == zwart ? enpassantBlack : enpassantWhite;}
void Game::set_enpassant_tile(ZW color, Tile position) {
    if(color == zwart)
        enpassantBlack = position;
    else
        enpassantWhite = position;
}

void Game::set_start_board() {
    // Load game state
    const BoardLayout& setup = GameConfig().getSetup();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            this->set_piece({i, j}, piece_from_character(setup[i][j], {i, j}));
}

SchaakStuk* Game::piece_from_character(char c, Tile position) const{
    // Create chesspieces based on their character representation in the initialSetup 2D array
    ZW color = std::islower(c) ? zwart : wit;
    switch (std::tolower(c)) {
        case 'r':
            return new Toren(color, position);
        case 'n':
            return new Paard(color, position);
        case 'b':
            return new Loper(color, position);
        case 'q':
            return new Koningin(color, position);
        case 'k':
            return new Koning(color, position);
        case 'p':
            return new Pion(color, position, std::islower(c) ? up : down);
        default:
            return nullptr;
    }
}

void Game::on_tile_click(ChessBoard* scene, Tile position, VisualOptions options) {
    SchaakStuk* pieceOnTarget = get_piece(position);
    // Player selected one of his own pieces so make it the current selected piece
    if(pieceOnTarget != nullptr && pieceOnTarget->get_color() == turn_){
        // Update the scene with visual information
        scene->removeAllMarking();
        scene->setTileSelect(position.first, position.second, true);
        selectedPiece_ = pieceOnTarget;
        update_tiles(scene, options);
        return;
    }
    // No piece has been selected and neither did the Player, so nothing can happen
    if(selectedPiece_ == nullptr)
        return;
    // Check en passant
    int rowDifference = std::abs(position.first - selectedPiece_->get_row());
    bool enpassant = (selectedPiece_->type() == pawn && rowDifference == 2);
    // Try to move the selected piece to the clicked Tile
    if(move(selectedPiece_, position)){
        // Update the scene in case the move was succesfully executed
        scene->update();
        scene->removeAllMarking();
        selectedPiece_ = nullptr;
        // Check for checkmate, check and stalemate
        if(checkmate(opposite(turn_)))
            std::cout << (turn_ == zwart ? "Black" : "White") << " wins!" << std::endl;
        else if(check(opposite(turn_)))
            std::cout << "Check!" << std::endl;
        else if(stalemate(opposite(turn_)))
            std::cout << "Draw!" << std::endl;
        // Update enpassant (-1 disables enpassant check)
        set_enpassant_tile(turn_, (enpassant ? position : Tile(-1,-1)));
        // Switch turn
        turn_ = opposite(turn_);
        // Update threatened pieces
        if(options.threatenedPieces)
            update_threatened_pieces(scene, options);
    }
}

Tiles Game::get_threatened_tiles(ZW color) {
    Tiles positions;
    for(auto &piece: get_pieces_of_color(color)){
        Tiles moves = piece->valid_moves(this);
        if(piece->type() == pawn)
            moves = ((Pion*) piece)->get_threats(this);
        for(const auto &move: moves)
            if(!vector_contains_tile(positions, move))
                positions.push_back(move);
    }
    return positions;
}

void Game::update_tiles(ChessBoard *scene, VisualOptions options) {
    if(selectedPiece_ == nullptr)
        return;
    Tiles moves = selectedPiece_->valid_moves(this);
    Tiles threats = get_threatened_tiles(opposite(turn_));
    for(const auto &move: moves){
        if(options.threats && !selectedPiece_->is_safe_move(this, move)){
            scene->setTileFocus(move.first, move.second, true);
            scene->setTileThreat(move.first, move.second, true);
        } else if(options.moves){
            scene->setTileFocus(move.first, move.second, true);
        }
    }
}

void Game::update_threatened_pieces(ChessBoard *scene, VisualOptions options) {
    if(!options.threatenedPieces)
        return;
    Tiles threats = get_threatened_tiles(opposite(turn_));
    for(const auto &piece: get_pieces_of_color(turn_))
        if(vector_contains_tile(threats, piece->get_position()))
            scene->setPieceThreat(piece->get_row(), piece->get_column(), true);
}

bool Game::move(SchaakStuk* s, Tile position) {
    // Checks if the move is valid. If not, return false
    if(!vector_contains_tile(s->valid_moves(this), position))
        return false;
    SchaakStuk* pieceOnTarget = get_piece(position);
    // Delete the en passant piece
    if(pieceOnTarget == nullptr && s->type() == pawn && s->get_column() != position.second){
        Tile enpassantTile = {s->get_row(), position.second};
        set_piece(enpassantTile, nullptr);
        delete get_piece(enpassantTile);
    }
    // Check king
    if(s->type() == king){
        ((Koning*) s)->set_moved(true);
        // Check if king rokades
        if(std::abs(s->get_column() - position.second) == 2){
            SchaakStuk* rook = get_piece({s->get_row(), s->get_column() < position.second ? 7 : 0});
            move(rook, {s->get_row(), std::abs((s->get_column()+position.second)/2)});
        }
    }
    // Delete piece on the target spot from memory
    delete pieceOnTarget;
    // Set the current Tile to a nullptr and the destination to the current piece
    set_piece(s->get_position(), nullptr);
    set_piece(position, s);
    // Update the piece's member variables row_ and column_
    s->set_position(position);
    return true;
}

Koning* Game::find_king(ZW color) const {
    // Iterate over own pieces
    for(const auto &piece: get_pieces_of_color(color))
        if(piece->type() == king)
            return (Koning*) piece;
    // There should always be a black and a white king on the board
    throw KingNotFoundException(color);
}

std::vector<Toren*> Game::find_rooks(ZW color) const {
    std::vector<Toren*> rooks;
    // Iterate over own pieces
    for(const auto &piece: get_pieces_of_color(color))
        if(piece->type() == rook)
            rooks.push_back((Toren*) piece);
    return rooks;
}

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
    Tiles threats;
    // Iterate over the opponents' pieces
    for(const auto &piece: get_pieces_of_color(opposite(color))){
        Tiles positions = piece->get_path_to(this, koning->get_position());
        if(positions.empty())
            continue;
        // Add threat positions to the threats Tiles
        for(const auto &position: positions)
            threats.push_back(position);
    }
    // Iterate over own pieces
    for(const auto &piece: get_pieces_of_color(color)) {
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

bool Game::move_prevents_checkmate(SchaakStuk* piece, Tile position) {
    // Backup
    SchaakStuk* backupPiece = get_piece(position);
    Tile backupPosition = piece->get_position();
    // Set pieces to simulation positions
    set_piece(backupPosition, nullptr);
    set_piece(position, piece);
    piece->set_position(position);
    // Check if it's still check
    bool stillCheck = check(piece->get_color());
    // Reset to initial positions
    set_piece(position, backupPiece);
    set_piece(backupPosition, piece);
    piece->set_position(backupPosition);
    return !stillCheck;
}

bool Game::stalemate(ZW color) {
    // It cannot be stalemate if the king is checked
    if(check(color))
        return false;
    // Check if at least one piece can still move
    for(const auto &piece: get_pieces_of_color(color))
        if(!piece->valid_moves(this).empty())
            return false;
    return true;
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

bool Game::vector_contains_tile(const Tiles &moves, Tile position) const{
    // Check if position is in moves
    return std::any_of(moves.begin(), moves.end(), [position](const Tile &move){ return move == position; });
}

ZW Game::opposite(ZW color) const {
    return color == zwart ? wit : zwart;
}

void Game::fill_board_with_nullpointers() {
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            bord_[i][j] = nullptr;
}
#pragma clang diagnostic pop
