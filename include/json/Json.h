#ifndef JSON_H
#define JSON_H

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>
#include <iostream>

class Json
{
private:
    using object_t = std::unordered_map<std::string, Json>;
    using array_t = std::vector<Json>;
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

    value_t value;

public:
    Json() : value(nullptr) {}
    Json(const string_t &val) : value(val) {}
    Json(const object_t &val) : value(val) {}
    Json(const array_t &val) : value(val) {}
    Json(const boolean_t &val) : value(val) {}
    Json(const number_integer_t &val) : value(val) {}
    Json(const number_float_t &val) : value(val) {}

    const value_t &get_value() const { return value; }
    const string_t &get_string() const { return std::get<string_t>(value); }
    const object_t &get_object() const { return std::get<object_t>(value); }
    const array_t &get_array() const { return std::get<array_t>(value); }
    const boolean_t &get_boolean() const { return std::get<boolean_t>(value); }
    const number_integer_t &get_number_integer() const { return std::get<number_integer_t>(value); }
    const number_float_t &get_number_float() const { return std::get<number_float_t>(value); }

    bool is_string() const { return std::holds_alternative<string_t>(value); }
    bool is_object() const { return std::holds_alternative<object_t>(value); }
    bool is_array() const { return std::holds_alternative<array_t>(value); }
    bool is_boolean() const { return std::holds_alternative<boolean_t>(value); }
    bool is_number_integer() const { return std::holds_alternative<number_integer_t>(value); }
    bool is_number_float() const { return std::holds_alternative<number_float_t>(value); }

    operator string_t() const { return get_string(); }
    operator object_t() const { return get_object(); }
    operator array_t() const { return get_array(); }
    operator boolean_t() const { return get_boolean(); }
    operator number_integer_t() const { return get_number_integer(); }
    operator number_float_t() const { return get_number_float(); }

    Json &operator[](const std::string &key)
    {
        if (!std::holds_alternative<object_t>(value))
        {
            value = object_t{};
        }
        if (!std::get<object_t>(value).count(key))
        {
            throw std::out_of_range("Key not found in JSON object");
        }
        return std::get<object_t>(value)[key];
    }
    Json &operator[](const char *key)
    {
        return (*this)[std::string(key)];
    }
    Json &operator=(const string_t &val)
    {
        value = val;
        return *this;
    }
    Json &operator=(boolean_t val)
    {
        value = val;
        return *this;
    }
    Json &operator=(number_integer_t val)
    {
        value = val;
        return *this;
    }
    Json &operator=(number_float_t val)
    {
        value = val;
        return *this;
    }
};

inline std::ostream &operator<<(std::ostream &os, const Json &json)
{
    if (json.is_string())
        os << json.get_string();
    else if (json.is_boolean())
        os << (json.get_boolean() ? "true" : "false");
    else if (json.is_number_integer())
        os << json.get_number_integer();
    else if (json.is_number_float())
        os << json.get_number_float();
    else if (json.is_array())
        os << "[Array]";
    else if (json.is_object())
        os << "{Object}";
    else
        os << "null";

    return os;
}

#endif // JSON_H