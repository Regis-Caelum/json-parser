#ifndef JsonValue_H
#define JsonValue_H

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <iostream>
#include <type_traits>
#include <utility>

template <typename>
inline constexpr bool always_false = false;

class JsonValue;

class JsonObject
{
public:
    JsonObject() = default;
    JsonObject(const JsonObject &other) = default;

    JsonValue &operator[](const std::string &key)
    {
        return object_[key];
    }

    JsonValue &operator[](const char *key)
    {
        return object_[std::string(key)];
    }

private:
    std::unordered_map<std::string, JsonValue> object_;
};

class JsonValue
{
public:
    using object_t = JsonObject;
    using array_t = std::vector<JsonValue>;
    using string_t = std::string;
    using boolean_t = bool;
    using number_integer_t = int64_t;
    using number_float_t = double;
    using value_t = std::variant<
        std::nullptr_t,
        string_t,
        object_t,
        array_t,
        boolean_t,
        number_integer_t,
        number_float_t>;

    JsonValue() : value_(nullptr) {}

    template <typename T>
    JsonValue(T &&val)
    {
        using DecayT = std::decay_t<T>;
        if constexpr (std::is_same_v<DecayT, std::nullptr_t>)
            value_ = nullptr;
        else if constexpr (std::is_same_v<DecayT, string_t>)
            value_ = std::forward<T>(val);
        else if constexpr (std::is_same_v<DecayT, const char *>)
            value_ = string_t(val);
        else if constexpr (std::is_same_v<DecayT, boolean_t>)
            value_ = val;
        else if constexpr (std::is_integral_v<DecayT> && !std::is_same_v<DecayT, bool>)
            value_ = static_cast<number_integer_t>(val);
        else if constexpr (std::is_floating_point_v<DecayT>)
            value_ = static_cast<number_float_t>(val);
        else if constexpr (std::is_same_v<DecayT, array_t> || std::is_same_v<DecayT, object_t>)
            value_ = val;
        else if constexpr (std::is_same_v<DecayT, JsonValue>)
            value_ = val.value_;
        else
            static_assert(always_false<DecayT>, "Unsupported type for JsonValue constructor");
    }

    JsonValue(std::initializer_list<JsonValue> list) : value_(array_t(list)) {}

    const value_t &get_value() const { return value_; }

    bool is_string() const { return std::holds_alternative<string_t>(value_); }
    bool is_object() const { return std::holds_alternative<object_t>(value_); }
    bool is_array() const { return std::holds_alternative<array_t>(value_); }
    bool is_boolean() const { return std::holds_alternative<boolean_t>(value_); }
    bool is_number_integer() const { return std::holds_alternative<number_integer_t>(value_); }
    bool is_number_float() const { return std::holds_alternative<number_float_t>(value_); }

    explicit operator string_t() const { return std::get<string_t>(value_); }
    explicit operator object_t() const { return std::get<object_t>(value_); }
    explicit operator array_t() const { return std::get<array_t>(value_); }
    explicit operator boolean_t() const { return std::get<boolean_t>(value_); }
    explicit operator number_integer_t() const { return std::get<number_integer_t>(value_); }
    explicit operator number_float_t() const { return std::get<number_float_t>(value_); }

    JsonValue &operator[](const std::string &key)
    {
        if (!is_object())
            value_ = object_t{};
        return std::get<object_t>(value_)[key];
    }

    JsonValue &operator[](const char *key) { return (*this)[std::string(key)]; }

    template <typename T>
    JsonValue &operator=(T &&val)
    {
        *this = JsonValue(std::forward<T>(val));
        return *this;
    }

private:
    value_t value_;
};

inline std::ostream &operator<<(std::ostream &os, const JsonValue &JsonValue)
{
    if (JsonValue.is_string())
        os << std::get<JsonValue::string_t>(JsonValue.get_value());
    else if (JsonValue.is_boolean())
        os << (std::get<JsonValue::boolean_t>(JsonValue.get_value()) ? "true" : "false");
    else if (JsonValue.is_number_integer())
        os << std::get<JsonValue::number_integer_t>(JsonValue.get_value());
    else if (JsonValue.is_number_float())
        os << std::get<JsonValue::number_float_t>(JsonValue.get_value());
    else if (JsonValue.is_array())
        os << "[Array]";
    else if (JsonValue.is_object())
        os << "{Object}";
    else
        os << "null";

    return os;
}

#endif // JsonValue_H
