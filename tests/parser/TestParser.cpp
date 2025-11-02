#include "parser/Parser.h"
#include "json/Json.h"

#include <gtest/gtest.h>

inline Token T(TokenType type, const std::string &value = "")
{
    return Token{type, value};
}

TEST(ParserTest, ParseEmptyObject)
{
    std::vector<Token> tokens = {T(TokenType::LBrace), T(TokenType::RBrace)};
    Parser parser(tokens);
    JsonObject result = parser.parse();
    EXPECT_TRUE(result.empty());
}

TEST(ParserTest, ParseSimpleObject)
{
    std::vector<Token> tokens = {
        T(TokenType::LBrace),
        T(TokenType::String, "name"),
        T(TokenType::Colon),
        T(TokenType::String, "John"),
        T(TokenType::Comma),
        T(TokenType::String, "age"),
        T(TokenType::Colon),
        T(TokenType::Number, "30"),
        T(TokenType::RBrace)};
    Parser parser(tokens);
    JsonObject result = parser.parse();

    EXPECT_TRUE(std::holds_alternative<std::string>(result["name"].get_value()));
    EXPECT_EQ(std::get<std::string>(result["name"].get_value()), "John");

    EXPECT_TRUE(std::holds_alternative<double>(result["age"].get_value()));
    EXPECT_EQ(std::get<double>(result["age"].get_value()), 30.0);
}

TEST(ParserTest, ParseNestedObject)
{
    std::vector<Token> tokens = {
        T(TokenType::LBrace),
        T(TokenType::String, "outer"),
        T(TokenType::Colon),
        T(TokenType::LBrace),
        T(TokenType::String, "inner"),
        T(TokenType::Colon),
        T(TokenType::Number, "42"),
        T(TokenType::RBrace),
        T(TokenType::RBrace)};
    Parser parser(tokens);
    JsonObject result = parser.parse();

    auto outer = std::get<JsonObject>(result["outer"].get_value());
    EXPECT_EQ(std::get<double>(outer["inner"].get_value()), 42.0);
}

TEST(ParserTest, ParseObjectWithArray)
{
    std::vector<Token> tokens = {
        T(TokenType::LBrace, "{"),
        T(TokenType::String, "numbers"),
        T(TokenType::Colon, ":"),
        T(TokenType::LBracket, "["),
        T(TokenType::Number, "1"),
        T(TokenType::Comma, ","),
        T(TokenType::Number, "2"),
        T(TokenType::Comma, ","),
        T(TokenType::Number, "3"),
        T(TokenType::RBracket, "]"),
        T(TokenType::RBrace, "}")};
    Parser parser(tokens);
    JsonObject result = parser.parse();

    auto arr = std::get<std::vector<JsonValue>>(result["numbers"].get_value());
    ASSERT_EQ(arr.size(), 3);
    EXPECT_EQ(std::get<double>(arr[0].get_value()), 1.0);
    EXPECT_EQ(std::get<double>(arr[1].get_value()), 2.0);
    EXPECT_EQ(std::get<double>(arr[2].get_value()), 3.0);
}

TEST(ParserTest, ParseComplexObjectAndArray)
{
    std::vector<Token> tokens = {
        T(TokenType::LBrace),
        T(TokenType::String, "data"),
        T(TokenType::Colon),
        T(TokenType::LBracket),
        T(TokenType::LBrace),
        T(TokenType::String, "x"),
        T(TokenType::Colon),
        T(TokenType::Number, "10"),
        T(TokenType::RBrace),
        T(TokenType::Comma),
        T(TokenType::LBrace),
        T(TokenType::String, "x"),
        T(TokenType::Colon),
        T(TokenType::Number, "20"),
        T(TokenType::RBrace),
        T(TokenType::RBracket),
        T(TokenType::RBrace)};
    Parser parser(tokens);
    JsonObject result = parser.parse();

    auto arr = std::get<std::vector<JsonValue>>(result["data"].get_value());
    EXPECT_EQ(arr.size(), 2);
    auto first = std::get<JsonObject>(arr[0].get_value());
    auto second = std::get<JsonObject>(arr[1].get_value());
    EXPECT_EQ(std::get<double>(first["x"].get_value()), 10.0);
    EXPECT_EQ(std::get<double>(second["x"].get_value()), 20.0);
}

TEST(ParserTest, ThrowsOnUnexpectedToken)
{
    std::vector<Token> tokens = {T(TokenType::Colon)};
    Parser parser(tokens);
    EXPECT_THROW(parser.parse(), std::runtime_error);
}

TEST(ParserTest, ThrowsOnUnclosedObject)
{
    std::vector<Token> tokens = {
        T(TokenType::LBrace),
        T(TokenType::String, "key"),
        T(TokenType::Colon),
        T(TokenType::Number, "1")};
    Parser parser(tokens);
    EXPECT_THROW(parser.parse(), std::runtime_error);
}
