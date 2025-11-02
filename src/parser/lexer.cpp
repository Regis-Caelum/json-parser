#include "parser/Lexer.h"

#include <sstream>

using namespace json;

void Lexer::skipWhitespace()
{
    while (!eof() && std::isspace(static_cast<unsigned char>(peek())))
        get();
}

std::vector<Token> Lexer::tokenise()
{
    std::vector<Token> tokens;
    Token token(TokenType::Invalid);
    if (input_.empty())
        return {Token(TokenType::EndOfFile)};

    while (!eof())
    {
        skipWhitespace();
        if (eof())
            break;

        token = nextToken();

        if (token.type == TokenType::Invalid)
            continue;

        tokens.push_back(token);
    }

    return tokens;
}

Token Lexer::nextToken()
{
    char c = peek();
    size_t start = pos_;

    switch (c)
    {
    case '{':
        get();
        return Token(TokenType::LBrace, "{", start);
    case '}':
        get();
        return Token(TokenType::RBrace, "}", start);
    case '[':
        get();
        return Token(TokenType::LBracket, "[", start);
    case ']':
        get();
        return Token(TokenType::RBracket, "]", start);
    case ':':
        get();
        return Token(TokenType::Colon, ":", start);
    case ',':
        get();
        return Token(TokenType::Comma, ",", start);
    case '"':
        return parseString();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return parseNumber();
    default:
        if (std::isalpha(static_cast<unsigned char>(c)))
            return parseLiteral();
        get();
        return Token(TokenType::Invalid, std::string(1, c), start);
    }
}

Token Lexer::parseString()
{
    size_t start = pos_;
    get(); // consume opening quote

    std::ostringstream value;
    bool escaped = false;

    while (!eof())
    {
        char c = get();

        if (escaped)
        {
            switch (c)
            {
            case '"':
                value << '"';
                break;
            case '\\':
                value << '\\';
                break;
            case '/':
                value << '/';
                break;
            case 'b':
                value << '\b';
                break;
            case 'f':
                value << '\f';
                break;
            case 'n':
                value << '\n';
                break;
            case 'r':
                value << '\r';
                break;
            case 't':
                value << '\t';
                break;
            case 'u':
                for (int i = 0; i < 4 && !eof(); ++i)
                    get();
                value << '?';
                break;
            default:
                value << c;
                break;
            }
            escaped = false;
        }
        else if (c == '\\')
        {
            escaped = true;
        }
        else if (c == '"')
        {
            return Token(TokenType::String, value.str(), start);
        }
        else
        {
            value << c;
        }
    }

    return Token(TokenType::Invalid, value.str(), start);
}

Token Lexer::parseNumber()
{
    size_t start = pos_;
    std::ostringstream value;

    if (peek() == '-')
        value << get();

    while (std::isdigit(peek()))
        value << get();

    if (peek() == '.')
    {
        value << get();
        while (std::isdigit(peek()))
            value << get();
    }

    if (peek() == 'e' || peek() == 'E')
    {
        value << get();
        if (peek() == '+' || peek() == '-')
            value << get();
        while (std::isdigit(peek()))
            value << get();
    }

    return Token(TokenType::Number, value.str(), start);
}

Token Lexer::parseLiteral()
{
    size_t start = pos_;
    std::string word;

    while (std::isalpha(peek()))
        word.push_back(get());

    if (word == "true")
        return Token(TokenType::True, word, start);
    if (word == "false")
        return Token(TokenType::False, word, start);
    if (word == "null")
        return Token(TokenType::Null, word, start);

    return Token(TokenType::Invalid, word, start);
}
