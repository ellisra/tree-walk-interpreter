#pragma once

#include <any>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include "token.h"

class Scanner {
    static const std::map<std::string, TokenType> keywords;

    std::string_view source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

public:
    Scanner(std::string_view source);
    std::vector<Token> scan_tokens();

private:
    void scan_token();
    void identifier();
    void number();
    void string();
    bool match(char expected);
    char peek();
    char peek_next();
    bool is_alpha(char c);
    bool is_alpha_numeric(char c);
    bool is_digit(char c);
    bool is_at_end();
    char advance();
    void add_token(TokenType type, std::any literal = nullptr);
};
