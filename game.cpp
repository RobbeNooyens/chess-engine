#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

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
    // Load initial game state
    const char initialSetup[8][8] =
            {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
             {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
             {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
             {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}};
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            this->set_piece(Tile(i, j), piece_from_character(initialSetup[i][j], Tile(i, j)));
}

SchaakStuk* Game::piece_from_character(char c, Tile position) const{
    int r = position.first, k = position.second;
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
    if(moves.empty())
        return false;
    return std::any_of(moves.begin(), moves.end(), [position](const Tile &move){
        return move.first == position.first && move.second == position.second;
    });
}

void Game::on_tile_click(ChessBoard* scene, Tile position) {
    SchaakStuk* pieceOnTarget = get_piece(position);
    // Player selected one of his own pieces so make it the current selected piece
    if(pieceOnTarget != nullptr && selected_piece_owner(pieceOnTarget) == turn_){
        // TODO: Check for stalemate
        // Update the scene with visual information
        scene->removeAllMarking();
        scene->setTileSelect(position.first, position.second, true);
        selectedPiece_ = pieceOnTarget;
        update_tiles(scene);
        return;
    }
    // No piece has been selected and neither did the player, so nothing can happen
    if(selectedPiece_ == nullptr)
        return;
    // Try to move the selected piece to the clicked Tile
    if(move(selectedPiece_, position)){
        // Update the scene in case the move was succesfully executed
        turn_ = (turn_ == black) ? white : black;
        scene->update();
        scene->removeAllMarking();
        selectedPiece_ = nullptr;
    }
}

void Game::update_tiles(ChessBoard *scene) {
    Tiles moves = selectedPiece_->valid_moves(this);
    for(const auto &move: moves)
        scene->setTileFocus(move.first, move.second, true);
}

player Game::selected_piece_owner(const SchaakStuk* s) const {
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
    // Update the piece's member variables row and column
    s->set_position(position);
    return true;
}

SchaakStuk* Game::find_king(ZW color) const{
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            SchaakStuk* piece = get_piece(Tile(i, j));
            if(piece == nullptr)
                continue;
            if(piece->get_color() == color && piece->piece().type() == piece->piece().King){
                return piece;
            }
        }
    }
    return nullptr;
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

    return false;
}

bool Game::draw(ZW) {
    return false;
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