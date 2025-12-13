#pragma once

#include <any>
#include <string>

#include "token_type.h"

class Token {
public:
    const TokenType type;
    const std::string lexme;
    const std::any literal;
    const int line;

    Token(TokenType type, std::string lexme, std::any literal, int line);

    std::string to_string() const;
};
