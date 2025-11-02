#include "parser/Parser.h"

using namespace json;

JsonObject Parser::parse()
{
    return parseObject();
}

const Token &Parser::current()
{
    if (pos_ >= tokens_.size())
    {
        static Token eof{TokenType::EndOfFile};
        return eof;
    }
    return tokens_[pos_];
}

void Parser::consume(TokenType expectedType)
{
    if (current().type != expectedType)
    {
        throw std::runtime_error("Unexpected token: " + current().value);
    }
    ++pos_;
}

JsonValue Parser::parseValue()
{
    switch (current().type)
    {
    case TokenType::LBrace:
        return parseObject();
    case TokenType::LBracket:
        return parseArray();
    case TokenType::String:
        return parseString();
    case TokenType::Number:
        return parseNumber();
    case TokenType::True:
    case TokenType::False:
    case TokenType::Null:
        return parseLiteral();
    default:
        throw std::runtime_error("Invalid JSON value: " + current().value);
    }
}

JsonObject Parser::parseObject()
{
    consume(TokenType::LBrace);
    JsonObject object;

    while (current().type != TokenType::RBrace)
    {
        if (current().type == TokenType::EndOfFile)
        {
            throw std::runtime_error("Unexpected end of input while parsing object");
        }

        std::string key = current().value;
        consume(TokenType::String);
        consume(TokenType::Colon);
        object[key] = parseValue();

        if (current().type == TokenType::Comma)
        {
            consume(TokenType::Comma);
        }
        else if (current().type != TokenType::RBrace)
        {
            throw std::runtime_error("Expected ',' or '}' in object");
        }
    }

    consume(TokenType::RBrace);
    return object;
}

JsonValue Parser::parseString()
{
    JsonValue str = JsonValue(current().value);
    consume(TokenType::String);
    return str;
}

JsonValue Parser::parseNumber()
{
    double num = std::stod(current().value);
    consume(TokenType::Number);
    return num;
}

JsonValue Parser::parseLiteral()
{
    bool b = (current().type == TokenType::True);
    if (b)
    {
        consume(current().type);
        return b;
    }
    b = (current().type == TokenType::False);
    if (b)
    {
        consume(current().type);
        return !b;
    }

    b = (current().type == TokenType::Null);
    if (b)
    {
        consume(current().type);
        return nullptr;
    }
    throw std::runtime_error("Invalid JSON value: " + current().value);
}

JsonValue Parser::parseArray()
{
    consume(TokenType::LBracket);
    std::vector<JsonValue> array;

    while (current().type != TokenType::RBracket && current().type != TokenType::EndOfFile)
    {
        array.push_back(parseValue());

        if (current().type == TokenType::Comma)
        {
            consume(TokenType::Comma);
        }
        else
        {
            break;
        }
    }

    consume(TokenType::RBracket);
    return array;
}
