//
// Created by robbe on 17-1-2021.
//

#include "MoveSimulation.h"
#include "game.h"
#include "SchaakStuk.h"

#include <utility>

MoveSimulation::MoveSimulation(Game* g, SchaakStuk* p, std::pair<int, int> tile): piece_(p), toTile_(std::move(tile)), game_(g) {
    // Backup
    backupPiece_ = game_->get_piece(tile);
    fromTile_ = piece_->get_position();
    // Set pieces to simulation positions
    game_->set_piece(fromTile_, nullptr);
    game_->set_piece(toTile_, piece_);
    piece_->set_position(toTile_);
}

void MoveSimulation::restore() {
    // Reset to initial positions
    game_->set_piece(toTile_, backupPiece_);
    game_->set_piece(fromTile_, piece_);
    piece_->set_position(fromTile_);
}
