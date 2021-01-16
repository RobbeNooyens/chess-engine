#include "promotepawn.h"
#include "ui_promotepawn.h"
#include "game.h"

PromotePawnDialog::PromotePawnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTest)
{
    ui->setupUi(this);
}

PromotePawnDialog::~PromotePawnDialog()
{
    delete ui;
}

void PromotePawnDialog::link_game(Game* game) {
    game_ = game;
}

void PromotePawnDialog::choose_promotion_piece(ChessBoard* scene, Tile position) {
    scene_ = scene;
    tile_ = position;
    show();
}

void PromotePawnDialog::on_rook_clicked()
{
    hide();
    this->game_->promote_piece_selected(rook, scene_, tile_);
}

void PromotePawnDialog::on_knight_clicked()
{
    hide();
    this->game_->promote_piece_selected(knight, scene_, tile_);
}

void PromotePawnDialog::on_bishop_clicked()
{
    hide();
    this->game_->promote_piece_selected(bishop, scene_, tile_);
}

void PromotePawnDialog::on_queen_clicked()
{
    hide();
    this->game_->promote_piece_selected(queen, scene_, tile_);
}
