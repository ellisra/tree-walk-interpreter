#include <any>
#include <string>
#include <utility>

#include "token.hpp"

Token::Token(TokenType type, std::string lexme, std::any literal, int line)
    : type{type}, lexme{std::move(lexme)}, literal{std::move(literal)}, line{line} {}

std::string Token::to_string() const {
    std::string literal_text;

    switch (type) {
        case (IDENTIFIER):
            literal_text = lexme;
            break;
        case (STRING):
            literal_text = std::any_cast<std::string>(literal);
            break;
        case (NUMBER):
            literal_text = std::to_string(std::any_cast<double>(literal));
            break;
        case (TRUE):
            literal_text = "true";
            break;
        case (FALSE):
            literal_text = "false";
            break;
        default:
            literal_text = "nil";
    }

    return ::to_string(type) + " " + lexme + " " + literal_text;
}
