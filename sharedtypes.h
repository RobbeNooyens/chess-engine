//
// Created by robbe on 15-1-2021.
//

#ifndef SCHAKEN_SHAREDTYPES_H
#define SCHAKEN_SHAREDTYPES_H

#include <vector>

class SchaakStuk;
class VisualOptions;
class ChessBoard;
class Game;
class Piece;
struct Direction;

typedef std::vector<std::pair<int,int>> Tiles;
typedef std::pair<int,int> Tile;
typedef std::vector<SchaakStuk*> Pieces;
typedef char BoardLayout[8][8];
typedef const std::vector<Direction> Directions;

enum BoardLayoutType{standard, endgame, promote, rokade};
enum PieceType{pawn, rook, knight, bishop, king, queen};
enum ZW{zwart,wit};
enum PawnDirection{up, down};

struct Direction {
    int rowRelative;
    int columnRelative;
    Direction(int r, int c) : rowRelative(r), columnRelative(c){}
    static Direction relative_direction(Tile start, Tile dest){
        int row = start.first > dest.first ? -1 : (start.first < dest.first ? 1 : 0);
        int column = start.second > dest.second ? -1 : (start.second < dest.second ? 1 : 0);
        return {row, column};
    }
    bool operator==(const Direction& other) const {
        return rowRelative == other.rowRelative && columnRelative == other.columnRelative;
    }
};

struct VisualOptions{
    bool moves;
    bool threats;
    bool threatenedPieces;
    VisualOptions(bool m, bool t, bool tP): moves(m), threats(t), threatenedPieces(tP){}
};

#endif //SCHAKEN_SHAREDTYPES_H
