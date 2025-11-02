#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"

class Parser
{
public:
    Parser(std::string input) : lexer_(input) {}
    std::vector<std::string> parse() { return lexer_.getTokens(); }
private:
    Lexer lexer_;
};

#endif // PARSER_H