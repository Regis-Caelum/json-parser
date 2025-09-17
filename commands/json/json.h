#pragma once
#include "../../command.h"

extern command json_command;

typedef enum
{
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

struct json_value;

typedef struct json_array
{
    struct json_value **items;
    size_t length;
} json_array;

typedef struct json_object_entry
{
    char *key;
    struct json_value *value;
} json_object_entry;

typedef struct json_object
{
    json_object_entry *entries;
    size_t length;
} json_object;

typedef struct json_value
{
    JsonType type;
    union
    {
        double number;
        int boolean;
        char *string;
        json_array array;
        json_object object;
    } u;
} json_value;
