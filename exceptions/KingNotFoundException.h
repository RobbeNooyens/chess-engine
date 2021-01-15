//
// Created by robbe on 14-1-2021.
//

#ifndef SCHAKEN_KINGNOTFOUNDEXCEPTION_H
#define SCHAKEN_KINGNOTFOUNDEXCEPTION_H


#include <exception>
#include <string>
#include <schaakstuk.h>

class KingNotFoundException: public std::exception {
public:
    explicit KingNotFoundException(ZW);
    const char* what() const noexcept override;
private:
    std::string message_;
};

KingNotFoundException::KingNotFoundException(const ZW color){
    message_.append("Could not find the ").append(color == zwart ? "black":"white").append(" King on the board.");
}

const char * KingNotFoundException::what() const noexcept {
    return message_.c_str();
}


#endif //SCHAKEN_KINGNOTFOUNDEXCEPTION_H
