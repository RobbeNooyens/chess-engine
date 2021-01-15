//
// Created by robbe on 15-1-2021.
//

#ifndef SCHAKEN_SHAREDTYPES_H
#define SCHAKEN_SHAREDTYPES_H

#include <vector>

class schaakstuk;
class VisualOptions;
class ChessBoard;

typedef std::vector<std::pair<int,int>> Tiles;
typedef std::pair<int,int> Tile;
typedef std::vector<schaakstuk*> Pieces;

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

#endif //SCHAKEN_SHAREDTYPES_H
