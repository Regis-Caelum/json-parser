#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType
{
    LBracket = 0,
    RBracket,
    LBrace,
    RBrace,
    Colon,
    Comma,

    True,
    False,
    Null,

    String,
    Number,

    EndOfFile,
    Invalid
};

class Token
{
public:
    TokenType type;
    std::string value;
    size_t position;

    Token(TokenType t, std::string v = "", size_t pos = 0)
        : type(t), value(std::move(v)), position(pos) {}
};

#endif // TOKEN_H