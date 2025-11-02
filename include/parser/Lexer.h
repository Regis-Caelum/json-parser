#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>

class Lexer
{
public:
    Lexer(std::string input) : input_(input) {}
    std::vector<std::string> getTokens()
    {
        if (tokens_.empty())
        {
            tokens_ = tokenise();
        }
        return tokens_;
    }

private:
    std::string input_;
    std::vector<std::string> tokens_;

    std::vector<std::string> tokenise();
};

#endif // LEXER_H