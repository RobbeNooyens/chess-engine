#ifndef DIALOGTEST_H
#define DIALOGTEST_H

#include <QDialog>

namespace Ui {
class DialogTest;
}

class Game;
class ChessBoard;
class VisualOptions;
class PromotePawnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PromotePawnDialog(QWidget *parent = nullptr);
    ~PromotePawnDialog();
    void link_game(Game*);
    void choose_promotion_piece(ChessBoard*, std::pair<int,int>);

private slots:
    void on_rook_clicked();

    void on_knight_clicked();

    void on_bishop_clicked();

    void on_queen_clicked();

private:
    Ui::DialogTest *ui;
    Game* game_;
    ChessBoard* scene_;
    std::pair<int,int> tile_;
};

#endif // DIALOGTEST_H
