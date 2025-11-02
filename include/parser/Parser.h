#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "json/Json.h"

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace json
{
    class Parser
    {
    public:
        Parser(std::vector<Token> tokens)
            : tokens_(tokens), pos_(0) {}

        JsonObject parse();

    private:
        std::vector<Token> tokens_;
        size_t pos_;

        const Token &current();
        void consume(TokenType expectedType);

        JsonValue parseValue();
        JsonObject parseObject();
        JsonValue parseArray();
        JsonValue parseString();
        JsonValue parseNumber();
        JsonValue parseLiteral();
    };
}

#endif // PARSER_H