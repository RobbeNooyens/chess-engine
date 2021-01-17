//
// Created by robbe on 17-1-2021.
//

#ifndef SCHAKEN_BOTCANTRESOLVECHECK_H
#define SCHAKEN_BOTCANTRESOLVECHECK_H

#include <exception>
#include <string>
#include "sharedtypes.h"

class BotCantResolveCheck: public std::exception {
public:
    explicit BotCantResolveCheck(ZW);
    const char* what() const noexcept override;
private:
    std::string message_;
};

BotCantResolveCheck::BotCantResolveCheck(ZW color) {
    message_.append("Bot couldn't resolve check for ").append(color == zwart ? "black.":"white.");
}

const char * BotCantResolveCheck::what() const noexcept {
    return message_.c_str();
}

#endif //SCHAKEN_BOTCANTRESOLVECHECK_H
