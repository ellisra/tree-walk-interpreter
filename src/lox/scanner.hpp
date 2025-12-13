#pragma once

#include <any>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "error.hpp"
#include "token.hpp"

class Scanner {
    static const std::map<std::string, TokenType> keywords;

    std::string_view source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

public:
    Scanner(std::string_view source): source{source} {}

    std::vector<Token> scan_tokens() {
        while (!is_at_end()) {
            start = current;
            scan_token();
        }

        tokens.emplace_back(END_OF_FILE, "", nullptr, line);
        return tokens;
    }

private:
    void scan_token() {
        char c = advance();
        switch (c) {
            case '(': add_token(LEFT_PAREN); break;
            case ')': add_token(RIGHT_PAREN); break;
            case '{': add_token(LEFT_BRACE); break;
            case '}': add_token(RIGHT_BRACE); break;
            case ',': add_token(COMMA); break;
            case '.': add_token(DOT); break;
            case '-': add_token(MINUS); break;
            case '+': add_token(PLUS); break;
            case ';': add_token(SEMICOLON); break;
            case '*': add_token(STAR); break;

            case '!':
                add_token(match('=') ? BANG_EQUAL : BANG);
                break;
            case '=':
                add_token(match('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<':
                add_token(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                add_token(match('=') ? GREATER_EQUAL : GREATER);
                break;

            case '/':
                if (match('/')) {
                    // Check if is a comment
                    while (peek() != '\n' && !is_at_end()) advance();
                } else {
                    add_token(SLASH);
                }
                break;

            // Ignore whitespace
            case ' ':
            case '\r':
            case '\t':
                break;

            case '\n':
                ++line;
                break;

            case '"': string(); break;

            default:
                if (is_digit(c)) {
                    number();
                } else if (is_alpha(c)) {
                    identifier();
                } else {
                    error(line, "Unexpected character.");
                }
                break;
        };
    }

    void identifier() {
        while (is_alpha_numeric(peek())) advance();
        std::string text = std::string{source.substr(start, current - start)};

        TokenType type;
        auto match = keywords.find(text);
        if (match == keywords.end()) {
            type = IDENTIFIER;
        } else {
            type = match->second;
        }

        add_token(type);
    }

    void number() {
        while (is_digit(peek())) advance();
        if (peek() == '.' && is_digit(peek_next())) {
            advance();
            while (is_digit(peek())) advance();
        }

        add_token(NUMBER, std::stod(std::string{source.substr(start, current - start)}));
    }

    void string() {
        while (peek() != '"' && !is_at_end()) {
            if (peek() == '\n') ++line;
            advance();
        }

        if (is_at_end()) {
            error(line, "Unterminated string.");
            return;
        }

        advance();
        std::string value{source.substr(start + 1, current - 2 - start)};
        add_token(STRING, value);
    }

    bool match(char expected) {
        if (is_at_end() || source[current] != expected) return false;
        ++current;

        return true;
    }

    char peek() {
        if (is_at_end()) return '\0';
        return source[current];
    }

    char peek_next() {
        if (current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }

    bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool is_alpha_numeric(char c) {
        return is_alpha(c) || is_digit(c);
    }

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool is_at_end() {
        return current >= source.length();
    }

    char advance() {
        return source[current++];
    }

    void add_token(TokenType type, std::any literal = nullptr) {
        std::string text{source.substr(start, current - start)};
        tokens.emplace_back(type, std::move(text), std::move(literal), line);
    }
};
