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


Game::Game() {}

Game::~Game() {
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(bord_[i][j] != nullptr)
                delete bord_[i][j];
        }
    }
}

SchaakStuk* Game::get_piece(Tile position) const {
    return bord_[position.first][position.second];
}

void Game::set_piece(Tile position, SchaakStuk*s) {
    bord_[position.first][position.second] = s;
}

void Game::set_start_board() {
    // Load game state
    const BoardLayout& setup = GameConfig().getSetup();
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            this->set_piece(Tile(i, j), piece_from_character(setup[i][j], Tile(i, j)));
}

SchaakStuk* Game::piece_from_character(char c, Tile position) const{
    int r = position.first, k = position.second;
    GameConfig config = GameConfig();
    // Create chesspieces based on their character representation in the initialSetup 2D array
    ZW color = std::islower(c) ? zwart : wit;
    switch (std::tolower(c)) {
        case 'r':
            return new Toren(color, r, k);
        case 'n':
            return new Paard(color, r, k);
        case 'b':
            return new Loper(color, r, k);
        case 'q':
            return new Koningin(color, r, k);
        case 'k':
            return new Koning(color, r, k);
        case 'p':
            return new Pion(color, r, k, std::islower(c) ? up : down);
        default:
            return nullptr;
    }
}

bool Game::is_valid_move(const Tile position, const Tiles& moves) const{
    // Check if position is in moves
    return std::any_of(moves.begin(), moves.end(), [position](const Tile &move){ return move == position; });
}

void Game::on_tile_click(ChessBoard* scene, Tile position) {
    SchaakStuk* pieceOnTarget = get_piece(position);
    // Player selected one of his own pieces so make it the current selected piece
    if(pieceOnTarget != nullptr && selected_piece_owner(pieceOnTarget) == turn_){
        // Update the scene with visual information
        scene->removeAllMarking();
        scene->setTileSelect(position.first, position.second, true);
        selectedPiece_ = pieceOnTarget;
        update_tiles(scene);
        return;
    }
    // No piece has been selected and neither did the Player, so nothing can happen
    if(selectedPiece_ == nullptr)
        return;
    // Try to move the selected piece to the clicked Tile
    if(move(selectedPiece_, position)){
        // Update the scene in case the move was succesfully executed
        scene->update();
        scene->removeAllMarking();
        selectedPiece_ = nullptr;
        // Check for checkmate, check and stalemate
        if(checkmate(turn_ == black ? wit : zwart))
            std::cout << (turn_ == black ? "Black" : "White") << " wins!" << std::endl;
        if(check(turn_ == black ? wit : zwart))
            std::cout << "Check!" << std::endl;
        if(stalemate(turn_ == black ? wit : zwart))
            std::cout << "Draw!" << std::endl;
        // Switch turn
        turn_ = (turn_ == black) ? white : black;
    }
}

void Game::update_tiles(ChessBoard *scene) {
    Tiles moves = selectedPiece_->valid_moves(this);
    for(const auto &move: moves)
        scene->setTileFocus(move.first, move.second, true);
}

Player Game::selected_piece_owner(const SchaakStuk* s) const {
    return s->get_color() == zwart ? black : white;
}

bool Game::move(SchaakStuk* s, Tile position) {
    // Checks if the move is valid. If not, return false
    if(!is_valid_move(position, s->valid_moves(this)))
        return false;
    // Delete de piece on the target spot from memory
    SchaakStuk* pieceOnTarget = get_piece(position);
    delete pieceOnTarget;
    // Set the current Tile to a nullptr and the destination to the current piece
    set_piece(Tile(s->get_row(), s->get_column()), nullptr);
    set_piece(position, s);
    // Update the piece's member variables row_ and column_
    s->set_position(position);
    return true;
}

SchaakStuk* Game::find_king(ZW color) const {
    // Iterate over own pieces
    for(const auto &piece: get_pieces_of_color(color))
        if(piece->piece().type() == piece->piece().King)
            return piece;
    // There should always be a black and a white king on the board
    throw KingNotFoundException(color);
}

bool Game::check(ZW color) const {
    // Find the King of the specified color
    SchaakStuk* king = find_king(color);
    // Iterate over all pieces with the other color
    for(const auto &piece: get_pieces_of_color(color == zwart ? wit : zwart))
        // If at least one piece can take the King, it's check
        if(is_valid_move(king->get_position(), piece->geldige_zetten(this)))
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
    for(const auto &piece: get_pieces_of_color(color == zwart ? wit : zwart)){
        Tiles positions = piece->get_path_to(this, koning->get_position());
        if(positions.empty()) {
            continue;
        }
        // Add threat positions to the threats Tiles
        for(const auto &position: positions) {
            threats.push_back(position);
        }
    }
    // Iterate over own pieces
    for(const auto &piece: get_pieces_of_color(color)) {
        if(piece->piece().type() == piece->piece().King) {
            continue;
        }
        // Iterate over every own pieces' moves
        for (const auto &move: piece->valid_moves(this)) {
            // Return if the piece can't be placed in between the mating piece and the king.
            if (!std::any_of(threats.begin(), threats.end(), [move](Tile threat) { return move == threat; })) {
                continue;
            }
            // Check if the piece can block the check
            if (move_prevents_checkmate(piece, move)) {
                return false;
            }
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
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            SchaakStuk* piece = get_piece(Tile(i, j));
            if(piece != nullptr)
                pieces.push_back(piece);
        }
    }
    return pieces;
}

Pieces Game::get_pieces_of_color(ZW color) const {
    Pieces pieces = get_pieces_on_board();
    pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [color](SchaakStuk* piece){return piece->get_color() != color;}), pieces.end());
    return pieces;
}
#pragma clang diagnostic pop