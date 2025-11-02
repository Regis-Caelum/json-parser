#ifndef LEXER_H
#define LEXER_H

#include "Token.h"

#include <vector>
#include <string>

class Lexer
{
public:
    explicit Lexer(const std::string &input)
        : input_(input), pos_(0) {}

    std::vector<Token> tokenise();

private:
    std::string input_;
    std::vector<char> punctuations_;
    size_t pos_;

    char peek() const { return pos_ < input_.size() ? input_[pos_] : '\0'; }
    char get() { return pos_ < input_.size() ? input_[pos_++] : '\0'; }
    bool eof() const { return pos_ >= input_.size(); }

    void skipWhitespace();
    Token nextToken();

    Token parseString();
    Token parseNumber();
    Token parseLiteral();
};

#endif // LEXER_H