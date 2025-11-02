#include "parser/Lexer.h"
#include <gtest/gtest.h>

using namespace json;

void assertTokens(const std::vector<Token> &tokens, const std::vector<TokenType> &expectedTypes, const std::vector<std::string> &expectedValues)
{
    ASSERT_EQ(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        EXPECT_EQ(tokens[i].type, expectedTypes[i]);
        EXPECT_EQ(tokens[i].value, expectedValues[i]);
    }
}

TEST(TestLexer, TokeniseEmptyInput)
{
    Lexer lexer("");
    auto tokens = lexer.tokenise();
    std::vector<TokenType> expectedTypes = {TokenType::EndOfFile};
    std::vector<std::string> expectedValues = {""};
    assertTokens(tokens, expectedTypes, expectedValues);
}

TEST(TestLexer, TokeniseSimpleWords)
{
    Lexer lexer("[\"hello\", \"world\"]");
    auto tokens = lexer.tokenise();

    std::vector<TokenType> expectedTypes = {
        TokenType::LBracket,
        TokenType::String,
        TokenType::Comma,
        TokenType::String,
        TokenType::RBracket};

    std::vector<std::string> expectedValues = {"[", "hello", ",", "world", "]", ""};

    assertTokens(tokens, expectedTypes, expectedValues);
}

TEST(TestLexer, TokeniseBasicObject)
{
    Lexer lexer("{\"key\":123}");
    auto tokens = lexer.tokenise();

    std::vector<TokenType> expectedTypes = {
        TokenType::LBrace,
        TokenType::String,
        TokenType::Colon,
        TokenType::Number,
        TokenType::RBrace};

    std::vector<std::string> expectedValues = {"{", "key", ":", "123", "}", ""};
    assertTokens(tokens, expectedTypes, expectedValues);
}

TEST(TestLexer, TokeniseEscapedString)
{
    Lexer lexer("[\"Hello \\\"World\\\"\"]"); // "Hello \"World\""
    auto tokens = lexer.tokenise();

    std::vector<TokenType> expectedTypes = {TokenType::LBracket, TokenType::String, TokenType::RBracket};
    std::vector<std::string> expectedValues = {"[", "Hello \"World\"", "]"};
    assertTokens(tokens, expectedTypes, expectedValues);
}

TEST(TestLexer, TokeniseStringWithSpaces)
{
    Lexer lexer("[\"Hello World\"]");
    auto tokens = lexer.tokenise();

    std::vector<TokenType> expectedTypes = {TokenType::LBracket, TokenType::String, TokenType::RBracket};
    std::vector<std::string> expectedValues = {"[", "Hello World", "]"};
    assertTokens(tokens, expectedTypes, expectedValues);
}

TEST(TestLexer, TokeniseNestedObjectsAndArrays)
{
    Lexer lexer("{\"outer\":{\"inner\":42},\"arr\":[1,2,3]}");
    auto tokens = lexer.tokenise();

    std::vector<TokenType> expectedTypes = {
        TokenType::LBrace,
        TokenType::String,
        TokenType::Colon,
        TokenType::LBrace,
        TokenType::String,
        TokenType::Colon,
        TokenType::Number,
        TokenType::RBrace,
        TokenType::Comma,
        TokenType::String,
        TokenType::Colon,
        TokenType::LBracket,
        TokenType::Number,
        TokenType::Comma,
        TokenType::Number,
        TokenType::Comma,
        TokenType::Number,
        TokenType::RBracket,
        TokenType::RBrace};

    std::vector<std::string> expectedValues = {
        "{", "outer", ":", "{", "inner", ":", "42", "}", ",",
        "arr", ":", "[", "1", ",", "2", ",", "3", "]", "}", ""};

    assertTokens(tokens, expectedTypes, expectedValues);
}

TEST(TestLexer, TokeniseMixedContent)
{
    Lexer lexer("{\"name\":\"John Doe\",\"age\":30,\"isStudent\":false,\"scores\":[100,98,95],\"nested\":{\"a\":1,\"b\":[true,false,null]}}");
    auto tokens = lexer.tokenise();

    EXPECT_EQ(tokens.front().type, TokenType::LBrace);
    EXPECT_EQ(tokens.back().type, TokenType::RBrace);

    EXPECT_EQ(tokens[23].value, "nested");
    EXPECT_EQ(tokens[30].value, "b");
}

TEST(TestLexer, TokeniseNumbersAndBooleans)
{
    Lexer lexer("[0,-1,3.14,6.022e23,true,false,null]");
    auto tokens = lexer.tokenise();

    std::vector<TokenType> expectedTypes = {
        TokenType::LBracket, TokenType::Number, TokenType::Comma,
        TokenType::Number, TokenType::Comma, TokenType::Number, TokenType::Comma,
        TokenType::Number, TokenType::Comma, TokenType::True, TokenType::Comma,
        TokenType::False, TokenType::Comma, TokenType::Null, TokenType::RBracket};

    std::vector<std::string> expectedValues = {
        "[", "0", ",", "-1", ",", "3.14", ",", "6.022e23", ",",
        "true", ",", "false", ",", "null", "]", ""};

    assertTokens(tokens, expectedTypes, expectedValues);
}
