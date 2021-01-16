//  Student: Robbe Nooyens
//  Rolnummer: 20201010
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include "game.h"
#include "chessboard.h"

// Constructors
SchaakStuk::SchaakStuk(ZW c, Tile t): kleur_(c), position_(std::move(t)) {}
Pion::Pion(ZW c, Tile t, PawnDirection d): SchaakStuk(c, t), moveDirection(d) {}
Toren::Toren(ZW c, Tile t): SchaakStuk(c, t) {}
Paard::Paard(ZW c, Tile t): SchaakStuk(c, t) {}
Loper::Loper(ZW c, Tile t): SchaakStuk(c, t) {}
Koning::Koning(ZW c, Tile t): SchaakStuk(c, t) {}
Koningin::Koningin(ZW c, Tile t): SchaakStuk(c, t) {}

// Piece wrappers
Piece Pion::piece() const {return {Piece::Pawn, get_color() == wit ? Piece::White : Piece::Black};}
Piece Toren::piece() const {return {Piece::Rook, get_color() == wit ? Piece::White : Piece::Black};}
Piece Paard::piece() const {return {Piece::Knight, get_color() == wit ? Piece::White : Piece::Black};}
Piece Loper::piece() const {return {Piece::Bishop, get_color() == wit ? Piece::White : Piece::Black};}
Piece Koning::piece() const {return {Piece::King, get_color() == wit ? Piece::White : Piece::Black};}
Piece Koningin::piece() const {return {Piece::Queen, get_color() == wit ? Piece::White : Piece::Black};}

// PieceTypes
PieceType Pion::type() const {return pawn;}
PieceType Toren::type() const {return rook;}
PieceType Paard::type() const {return knight;}
PieceType Loper::type() const {return bishop;}
PieceType Koningin::type() const {return queen;}
PieceType Koning::type() const {return king;}

// Getters and Setters
ZW SchaakStuk::get_color() const {return kleur_;}
int SchaakStuk::get_row() const {return position_.first;}
int SchaakStuk::get_column() const {return position_.second;}
Tile SchaakStuk::get_position() const {return position_;}
void SchaakStuk::set_position(Tile position) {position_ = std::move(position);}

bool SchaakStuk::can_move_to(const Game* game, Tile position) const{
    // Check if piece is off the board
    int r = position.first; int c = position.second;
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Check if the Tile at the specified coordinates is empty
    return game->get_piece(position) == nullptr;
}

bool SchaakStuk::can_take_at(const Game* game, Tile position) const{
    // Check if piece is off the board
    int r = position.first; int c = position.second;
    if(r < 0 || r >= 8 || c < 0 || c >= 8)
        return false;
    // Get the piece at the specified coordinates
    SchaakStuk* piece = game->get_piece(position);
    if(piece == nullptr)
        return false;
    return piece->get_color() != kleur_;
}

Tiles SchaakStuk::moves_from_directions(const Game* game, Directions &directions) const{
    Tiles moves;
    // For every directions, start from current pos and follow the PawnDirection until it can no longer move further
    for(const auto direction: directions){
        // Start from the current Tile with the directions being added once
        int rowAbsolute = get_row() + direction.rowRelative;
        int columnAbsolute = get_column() + direction.columnRelative;
        // Add Tiles until it hits another piece
        while(this->can_move_to(game, Tile(rowAbsolute, columnAbsolute))){
            moves.emplace_back(rowAbsolute, columnAbsolute);
            rowAbsolute += direction.rowRelative;
            columnAbsolute += direction.columnRelative;
        }
        // Check if the piece can take the piece it ended on
        if(this->can_take_at(game, Tile(rowAbsolute, columnAbsolute)))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

Tiles SchaakStuk::moves_from_positions(const Game *game, Directions &directions) const {
    Tiles moves;
    for(const auto &direction: directions){
        int rowAbsolute = get_row() + direction.rowRelative;
        int columnAbsolute = get_column() + direction.columnRelative;
        if(can_move_to(game, Tile(rowAbsolute, columnAbsolute)) || can_take_at(game, Tile(rowAbsolute, columnAbsolute)))
            moves.emplace_back(rowAbsolute, columnAbsolute);
    }
    return moves;
}

Tiles SchaakStuk::path_to_target(const Game* game, const Tile target, Directions &directions) const {
    // Add own position to the Tiles
    Tiles path = {get_position()};
    // Figure out the relative direction with row_, column_ elements of {-1, 0, 1}
    Direction direction = Direction::relative_direction(get_position(), target);
    // Check if direction is a move for this piece
    if(std::find_if(directions.begin(), directions.end(), [direction](Direction move){return direction == move;}) == directions.end())
        return path;
    int rowAbsolute = get_row() + direction.rowRelative;
    int columnAbsolute = get_column() + direction.columnRelative;
    // Move in the direction until onather piece has been reached
    while(can_move_to(game, Tile(rowAbsolute, columnAbsolute))){
        path.emplace_back(rowAbsolute, columnAbsolute);
        rowAbsolute += direction.rowRelative;
        columnAbsolute += direction.columnRelative;
    }
    // Return the path if the tile it ended on is the target
    return (Tile(rowAbsolute, columnAbsolute) == target) ? path : Tiles();
}

bool SchaakStuk::is_pinned(Game* game, Tile targetPos) {
    // Backup
    SchaakStuk* pieceOnTarget = game->get_piece(targetPos);
    Tile initPosition = get_position();
    // Simulate move
    game->set_piece(initPosition, nullptr);
    game->set_piece(targetPos, this);
    set_position(targetPos);
    bool pin = game->check(kleur_);
    // Reset state
    game->set_piece(targetPos, pieceOnTarget);
    game->set_piece(initPosition, this);
    set_position(initPosition);
    return pin;
}

void SchaakStuk::remove_pinned_moves(Game* game, Tiles& moves) {
    // Remove moves that lead to check
    moves.erase(std::remove_if(moves.begin(), moves.end(), [this, game](Tile move){return is_pinned(game, move);}), moves.end());
}

bool SchaakStuk::is_safe_move(Game* game, Tile targetPos) {
    // Backup
    SchaakStuk* pieceOnTarget = game->get_piece(targetPos);
    Tile initPosition = get_position();
    // Simulate move
    game->set_piece(initPosition, nullptr);
    game->set_piece(targetPos, this);
    set_position(targetPos);
    bool safe = !game->vector_contains_tile(game->get_threatening_tiles(get_color() == zwart ? wit : zwart), targetPos);
    // Reset state
    game->set_piece(targetPos, pieceOnTarget);
    game->set_piece(initPosition, this);
    set_position(initPosition);
    return safe;
}

/**
 * Returns all valid moves with pin check
 * @param game
 * @return
 */
Tiles SchaakStuk::valid_moves(Game* game) {
    Tiles moves = this->geldige_zetten(game);
    // Filters out all moves that will lead to check
    remove_pinned_moves(game, moves);
    return moves;
}
Tiles Pion::get_threats(const Game *game) {
    int row = this->get_row(), column = this->get_column();
    Tiles moves;
    int dirRelative = ((moveDirection == up) ? -1 : 1);
    int nextRow = row + dirRelative;
    // Check if pawn can take a piece sideways
    if(this->can_take_at(game, Tile(nextRow, column - 1)))
        moves.emplace_back(nextRow, column-1);
    if(this->can_take_at(game, Tile(nextRow, column + 1)))
        moves.emplace_back(nextRow, column+1);
    return moves;
}
// Returns naive possible moves without pin check
Tiles Pion::geldige_zetten(const Game* game) const {
    int row = this->get_row(), column = this->get_column();
    Tiles moves;
    int dirRelative = ((moveDirection == up) ? -1 : 1);
    int nextRow = row + dirRelative;

    // Check if move forward is possible
    if (this->can_move_to(game, Tile(nextRow, column))) {
        moves.emplace_back(nextRow, column);
        // Check if pawn can move two squares
        if (row == (moveDirection == up ? 6 : 1) && this->can_move_to(game, Tile(nextRow + dirRelative, column)))
            moves.emplace_back(nextRow + dirRelative, column);
    }

    // Check if pawn can take a piece sideways or en passant
    Tile enpassantTile = game->get_enpassant_tile(game->opposite(get_color()));
    if(this->can_take_at(game, Tile(nextRow, column - 1)) || Tile(row, column-1) == enpassantTile)
        moves.emplace_back(nextRow, column-1);
    if(this->can_take_at(game, Tile(nextRow, column + 1)) || Tile(row, column+1) == enpassantTile)
        moves.emplace_back(nextRow, column+1);

    return moves;
}
Tiles Toren::geldige_zetten(const Game* game) const {
    // Directions: down, up, right,
    // | | |X| | |
    // | | |X| | |
    // |X|X|R|X|X|
    // | | |X| | |
    // | | |X| | |
    return moves_from_directions(game, directions_);
}
Tiles Paard::geldige_zetten(const Game* game) const {
    // Directions specific to the Knight: 2 to one axis, 1 to the other axis
    // | |X| |X| |
    // |X| | | |X|
    // | | |N| | |
    // |X| | | |X|
    // | |X| |X| |
    return moves_from_positions(game, directions_);
}
Tiles Loper::geldige_zetten(const Game* game) const {
    // Directions: down-right, down-left, up-left, up-right
    // |X| | | |X|
    // | |X| |X| |
    // | | |B| | |
    // | |X| |X| |
    // |X| | | |X|
    return moves_from_directions(game, directions_);
}
Tiles Koningin::geldige_zetten(const Game* game) const {
    // Directions: down, up, right, left, down-right, down-left, up-left, up-right
    // |X| |X| |X|
    // | |X|X|X| |
    // |X|X|Q|X|X|
    // | |X|X|X| |
    // |X| |X| |X|
    return moves_from_directions(game, directions_);
}
Tiles Koning::geldige_zetten(const Game* game) const {
    // Directions: Square around the current Tile
    // | | | | | |
    // | |X|X|X| |
    // | |X|K|X| |
    // | |X|X|X| |
    // | | | | | |
    return moves_from_positions(game, directions_);
}

// Get path to the king
Tiles Pion::get_path_to(const Game* game, Tile position) const { return {get_position()}; }
Tiles Paard::get_path_to(const Game* game, Tile position) const { return {get_position()}; }
Tiles Koning::get_path_to(const Game* game, Tile position) const { return {get_position()}; }
Tiles Toren::get_path_to(const Game* game, Tile position) const { return path_to_target(game, position, directions_); }
Tiles Loper::get_path_to(const Game* game, Tile position) const { return path_to_target(game, position, directions_); }
Tiles Koningin::get_path_to(const Game* game, Tile position) const { return path_to_target(game, position, directions_); }