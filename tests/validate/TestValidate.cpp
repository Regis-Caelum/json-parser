#include "parser/Lexer.h"
#include "parser/Validate.h"

#include <gtest/gtest.h>

using namespace json;

TEST(ValidatorTest, ValidSimpleObject)
{
    std::string input = "{ \"key\": 123 }";
    Lexer lexer(input);
    EXPECT_TRUE(validator::validate(lexer));
}

TEST(ValidatorTest, ValidNestedObject)
{
    std::string input = "{ \"outer\": { \"inner\": 42 }, \"arr\": [1, 2, 3] }";
    Lexer lexer(input);
    EXPECT_TRUE(validator::validate(lexer));
}

TEST(ValidatorTest, ValidArrayOfValues)
{
    std::string input = "[1, 2, 3, 4, 5]";
    Lexer lexer(input);
    EXPECT_TRUE(validator::validate(lexer));
}

TEST(ValidatorTest, ValidEmptyObjectAndArray)
{
    Lexer lexer1("{}");
    Lexer lexer2("[]");
    EXPECT_TRUE(validator::validate(lexer1));
    EXPECT_TRUE(validator::validate(lexer2));
}

TEST(ValidatorTest, ValidStringWithEscapes)
{
    std::string input = "{ \"text\": \"Hello \\\"World\\\"\" }";
    Lexer lexer(input);
    EXPECT_TRUE(validator::validate(lexer));
}

// ================= Invalid cases =================

TEST(ValidatorTest, InvalidUnmatchedBraces)
{
    std::string input = "{ \"key\": 123 ";
    Lexer lexer(input);
    EXPECT_FALSE(validator::validate(lexer));
}

TEST(ValidatorTest, InvalidUnmatchedBrackets)
{
    std::string input = "[1, 2, 3";
    Lexer lexer(input);
    EXPECT_FALSE(validator::validate(lexer));
}

TEST(ValidatorTest, InvalidColonPosition)
{
    std::string input = "{ 123: \"value\" }"; // key must be string
    Lexer lexer(input);
    EXPECT_FALSE(validator::validate(lexer));
}

TEST(ValidatorTest, InvalidCommaPosition)
{
    std::string input = "{ \"key\": 123 , : 456 }";
    Lexer lexer(input);
    EXPECT_FALSE(validator::validate(lexer));
}

TEST(ValidatorTest, InvalidExtraComma)
{
    std::string input = "[1, 2, 3, ]"; // trailing comma
    Lexer lexer(input);
    EXPECT_FALSE(validator::validate(lexer));
}

TEST(ValidatorTest, ComplexValidJSON)
{
    std::string input = R"({
        "name": "John",
        "age": 30,
        "isStudent": false,
        "scores": [100, 98, 95],
        "address": {"city": "NY", "zip": "10001"}
    })";
    Lexer lexer(input);
    EXPECT_TRUE(validator::validate(lexer));
}

TEST(ValidatorTest, EmptyInput)
{
    Lexer lexer("");
    EXPECT_TRUE(validator::validate(lexer)); // technically empty input is valid
}
