//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <chessboard.h>
#include <vector>
#include <game.h>

enum zw{zwart,wit};
enum direction{up, down};

class SchaakStuk {
public:
    SchaakStuk(zw kleur): kleur(kleur) {}

    virtual Piece piece() const=0;
    virtual std::vector<std::pair<int,int>> valid_moves(Game &game) const= 0;

    zw getKleur() const {return kleur;}
    int getRow() const {return row;}
    int getColumn() const {return column;}

    void updatePosition(int r, int c) {
        this->row = r;
        this->column = c;
    }

    bool canMoveTo(Game &game, int r, int c){
        if(r < 0 || r >= 8 || c < 0 || c >= 8)
            return false;
        return game.getPiece(r, c) != nullptr;
    }

    bool canTakeAt(Game &game, int r, int c){
        if(r < 0 || r >= 8 || c < 0 || c >= 8)
            return false;
        SchaakStuk* piece = game.getPiece(r, c);
        if(piece == nullptr)
            return false;
        return piece.getKleur() != kleur;
    }
private:
    zw kleur;
    int row;
    int column;
};

class Pion:public SchaakStuk {
public:
    Pion(zw kleur, direction dir): SchaakStuk(kleur), moveDirection(dir) {}
    virtual Piece piece() const override {
        return Piece(Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black);
    }
    std::vector<std::pair<int,int>> valid_moves(Game &game) const override {
        std::vector<std::pair<int,int>> moves;
        int dirRelative = moveDirection == up ? -1 : 1;
        int nextRow = this->getRow() + dirRelative;
        // Check if move forward is possible
        if (game.getPiece(nextRow, this->getColumn()) == nullptr)
            moves.push_back(std::make_pair(nextRow, this->getColumn()));
        // Check if pawn can take a piece sideways

        SchaakStuk* pieceToTake = game.getPiece(nextRow, this->getColumn() + 1);
        if (pieceToTake != nullptr && pieceToTake.getKleur() != getKleur())
            moves.push_back(std::make_pair(nextRow, this->getColumn() + 1));
        if (game.getPiece(nextRow, this->getColumn() - 1) == nullptr)
            moves.push_back(std::make_pair(nextRow, this->getColumn()));
        if(this->getRow() == (moveDirection == up ? 6 : 1)){
            // Piece can also move 2 squares up
        }
        if(moveDirection == up){

        }
        return nullptr;
    }
private:
    direction moveDirection;
};

class Toren:public SchaakStuk {
public:
    Toren(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Rook,getKleur()==wit?Piece::White:Piece::Black);
    }

    std::vector<std::pair<int,int>> valid_moves(Game &game) const override {
        return nullptr;
    }
};

class Paard:public SchaakStuk {
public:
    Paard(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Knight,getKleur()==wit?Piece::White:Piece::Black);
    }

    std::vector<std::pair<int,int>> valid_moves(Game &game) const override {
        return nullptr;
    }
};

class Loper:public SchaakStuk {
public:
    Loper(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black);
    }

    std::vector<std::pair<int,int>> valid_moves(Game &game) const override {
        return nullptr;
    }
};

class Koning:public SchaakStuk {
public:
    Koning(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::King,getKleur()==wit?Piece::White:Piece::Black);
    }

    std::vector<std::pair<int,int>> valid_moves(Game &game) const override {
        return nullptr;
    }
};

class Koningin:public SchaakStuk {
public:
    Koningin(zw kleur):SchaakStuk(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Queen,getKleur()==wit?Piece::White:Piece::Black);
    }

    std::vector<std::pair<int,int>> valid_moves(Game &game) const override {
        return nullptr;
    }
};

#endif //SCHAKEN_SCHAAKSTUK_H
