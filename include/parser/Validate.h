#ifndef VALIDATE_H
#define VALIDATE_H

#include "Lexer.h"

#include <stack>

namespace validator
{
    inline bool validate(Lexer &lexer)
    {
        auto tokens = lexer.tokenise();
        std::stack<TokenType> stack;
        TokenType prev = TokenType::Null;

        for (size_t i = 0; i < tokens.size(); ++i)
        {
            const auto &token = tokens[i];

            if (token.type == TokenType::Invalid)
                return false;

            if (token.type == TokenType::LBrace)
            {
                stack.push(TokenType::LBrace);
            }
            else if (token.type == TokenType::RBrace)
            {
                if (stack.empty() || stack.top() != TokenType::LBrace)
                    return false;
                stack.pop();
            }
            else if (token.type == TokenType::LBracket)
            {
                stack.push(TokenType::LBracket);
            }
            else if (token.type == TokenType::RBracket)
            {
                if (stack.empty() || stack.top() != TokenType::LBracket ||
                    (prev == TokenType::Colon || prev == TokenType::Comma))
                    return false;
                stack.pop();
            }
            else if (token.type == TokenType::Colon)
            {
                if (prev != TokenType::String)
                    return false;
            }
            else if (token.type == TokenType::Comma)
            {
                if (!(prev == TokenType::String || prev == TokenType::Number ||
                      prev == TokenType::True || prev == TokenType::False ||
                      prev == TokenType::Null || prev == TokenType::RBrace ||
                      prev == TokenType::RBracket))
                    return false;
            }

            prev = token.type;
        }

        return stack.empty();
    }
}
#endif // VALIDATE_H