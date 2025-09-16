#pragma once
#include "../../command.h"

extern Command JsonCommand;

typedef enum
{
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

struct JsonValue;

typedef struct JsonArray
{
    struct JsonValue **items;
    size_t length;
} JsonArray;

typedef struct JsonObjectEntry
{
    char *key;
    struct JsonValue *value;
} JsonObjectEntry;

typedef struct JsonObject
{
    JsonObjectEntry *entries;
    size_t length;
} JsonObject;

typedef struct JsonValue
{
    JsonType type;
    union
    {
        double number;
        int boolean;
        char *string;
        JsonArray array;
        JsonObject object;
    } u;
} JsonValue;
