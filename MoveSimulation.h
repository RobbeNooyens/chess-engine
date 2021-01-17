//
// Created by robbe on 17-1-2021.
//

#ifndef SCHAKEN_MOVESIMULATION_H
#define SCHAKEN_MOVESIMULATION_H

#include <utility>

class SchaakStuk;
class Game;
class MoveSimulation {
public:
    MoveSimulation(Game*, SchaakStuk*, std::pair<int,int>);
    void restore();
private:
    Game* game_;
    SchaakStuk* piece_;
    SchaakStuk* backupPiece_;
    std::pair<int,int> fromTile_;
    std::pair<int,int> toTile_;
};


#endif //SCHAKEN_MOVESIMULATION_H
