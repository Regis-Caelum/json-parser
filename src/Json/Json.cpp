#include "json/Json.h"
#include "parser/Parser.h"
#include "parser/Lexer.h"

#include <sstream>

namespace json
{

    std::string jsonEncode(const JsonValue &value);
    
    std::string jsonEncode(const JsonObject &jsonObj);

    std::string jsonEncode(const JsonObject &jsonObj)
    {
        std::ostringstream oss;
        oss << "{";
        bool first = true;

        for (const auto &pair : jsonObj)
        {
            if (!first)
                oss << ",";
            first = false;

            oss << "\"" << pair.first << "\":";
            oss << jsonEncode(pair.second);
        }

        oss << "}";
        return oss.str();
    }

    std::string jsonEncode(const JsonValue &value)
    {
        const auto &v = value.get_value();

        if (std::holds_alternative<std::nullptr_t>(v))
        {
            return "null";
        }
        else if (std::holds_alternative<std::string>(v))
        {
            return "\"" + std::get<std::string>(v) + "\"";
        }
        else if (std::holds_alternative<double>(v))
        {
            return std::to_string(std::get<double>(v));
        }
        else if (std::holds_alternative<bool>(v))
        {
            return std::get<bool>(v) ? "true" : "false";
        }
        else if (std::holds_alternative<JsonObject>(v))
        {
            return jsonEncode(std::get<JsonObject>(v));
        }
        else if (std::holds_alternative<std::vector<JsonValue>>(v))
        {
            const auto &arr = std::get<std::vector<JsonValue>>(v);
            std::ostringstream oss;
            oss << "[";
            for (size_t i = 0; i < arr.size(); ++i)
            {
                if (i > 0)
                    oss << ",";
                oss << jsonEncode(arr[i]);
            }
            oss << "]";
            return oss.str();
        }

        throw std::runtime_error("Unsupported JsonValue type");
    }

    JsonObject jsonDecode(std::string_view jsonStr)
    {
        Lexer lexer(jsonStr);
        Parser parser(lexer.tokenise());
        return parser.parse();
    }

}