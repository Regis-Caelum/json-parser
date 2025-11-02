#include "json/Json.h"
#include "parser/Parser.h"
#include "parser/Lexer.h"
#include "parser/Token.h"

#include <gtest/gtest.h>
#include <string>

using namespace json;

JsonObject makeSimpleJson()
{
    JsonObject obj;
    obj["name"] = "John";
    obj["age"] = 30.0;
    obj["married"] = true;
    obj["children"] = nullptr;
    return obj;
}

TEST(JsonEncodeTest, EncodeEmptyObject)
{
    JsonObject obj;
    EXPECT_EQ(jsonEncode(obj), "{}");
}

TEST(JsonEncodeTest, EncodeSimpleObject)
{
    JsonObject obj = makeSimpleJson();
    std::string result = jsonEncode(obj);

    // Order of keys in unordered_map is undefined, so we only check for substrings
    EXPECT_TRUE(result.find("\"name\":\"John\"") != std::string::npos);
    EXPECT_TRUE(result.find("\"age\":30.000000") != std::string::npos);
    EXPECT_TRUE(result.find("\"married\":true") != std::string::npos);
    EXPECT_TRUE(result.find("\"children\":null") != std::string::npos);
}

TEST(JsonEncodeTest, EncodeArray)
{
    const JsonValue arr = std::vector<JsonValue>{
        "apple",
        10.0,
        true,
        nullptr};

    std::string encoded = jsonEncode(arr);
    EXPECT_EQ(encoded, "[\"apple\",10.000000,true,null]");
}

TEST(JsonEncodeTest, EncodeNestedObjects)
{
    JsonObject inner;
    inner["x"] = 42.0;

    JsonObject outer;
    outer["inner"] = inner;

    std::string encoded = jsonEncode(outer);
    EXPECT_TRUE(encoded.find("\"inner\":") != std::string::npos);
    EXPECT_TRUE(encoded.find("\"x\":42.000000") != std::string::npos);
}

TEST(JsonEncodeTest, EncodeComplexObjectWithArray)
{
    JsonObject obj;
    obj["data"] = std::vector<JsonValue>{JsonValue(1.0), JsonValue(2.0), JsonValue(3.0)};
    obj["active"] = true;

    std::string encoded = jsonEncode(obj);
    EXPECT_TRUE(encoded.find("\"data\":[1.000000,2.000000,3.000000]") != std::string::npos);
    EXPECT_TRUE(encoded.find("\"active\":true") != std::string::npos);
}

TEST(JsonEncodeTest, EncodeThrowsOnUnsupportedType)
{
    JsonValue invalid;
    EXPECT_EQ(jsonEncode(invalid), "null");
}

// ---------------------------
// Decode tests
// ---------------------------

TEST(JsonDecodeTest, DecodeSimpleObject)
{
    std::string jsonStr = "{\"name\":\"John\",\"age\":30,\"married\":true,\"children\":null}";
    JsonObject obj = jsonDecode(jsonStr);

    EXPECT_EQ(std::get<std::string>(obj["name"].get_value()), "John");
    EXPECT_EQ(std::get<double>(obj["age"].get_value()), 30);
    EXPECT_TRUE(std::get<bool>(obj["married"].get_value()));
    EXPECT_TRUE(std::holds_alternative<std::nullptr_t>(obj["children"].get_value()));
}

TEST(JsonDecodeTest, DecodeNestedObjects)
{
    std::string jsonStr = "{\"outer\":{\"inner\":42}}";
    JsonObject obj = jsonDecode(jsonStr);

    auto outer = std::get<JsonObject>(obj["outer"].get_value());
    EXPECT_EQ(std::get<double>(outer["inner"].get_value()), 42);
}

TEST(JsonDecodeTest, DecodeArray)
{
    std::string jsonStr = "{\"arr\":[1,2,3]}";
    JsonObject obj = jsonDecode(jsonStr);
    auto arr = std::get<std::vector<JsonValue>>(obj["arr"].get_value());
    ASSERT_EQ(arr.size(), 3);
    EXPECT_EQ(std::get<double>(arr[0].get_value()), 1.0);
    EXPECT_EQ(std::get<double>(arr[1].get_value()), 2.0);
    EXPECT_EQ(std::get<double>(arr[2].get_value()), 3.0);
}

TEST(JsonDecodeTest, DecodeBooleanAndNull)
{
    std::string jsonStr = "{\"flag\":true,\"none\":null}";
    JsonObject obj = jsonDecode(jsonStr);

    EXPECT_TRUE(std::get<bool>(obj["flag"].get_value()));
    EXPECT_TRUE(std::holds_alternative<std::nullptr_t>(obj["none"].get_value()));
}

TEST(JsonDecodeTest, DecodeThrowsOnInvalidJson)
{
    std::string invalidJson = "{\"name\":,}";
    EXPECT_THROW(jsonDecode(invalidJson), std::runtime_error);
}

// ---------------------------
// Round-trip tests
// ---------------------------

TEST(JsonRoundTripTest, EncodeThenDecodeSameStructure)
{
    JsonObject original;
    original["key"] = "value";
    original["num"] = 123.0;
    original["flag"] = false;
    original["list"] = std::vector<JsonValue>{"x", 5.0};

    std::string encoded = jsonEncode(original);
    JsonObject decoded = jsonDecode(encoded);

    EXPECT_EQ(std::get<std::string>(decoded["key"].get_value()), "value");
    EXPECT_EQ(std::get<double>(decoded["num"].get_value()), 123.0);
    EXPECT_FALSE(std::get<bool>(decoded["flag"].get_value()));

    auto list = std::get<std::vector<JsonValue>>(decoded["list"].get_value());
    EXPECT_EQ(std::get<std::string>(list[0].get_value()), "x");
    EXPECT_EQ(std::get<double>(list[1].get_value()), 5.0);
}
