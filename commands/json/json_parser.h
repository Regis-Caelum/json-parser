#pragma once
#include "json.h"

json_object *parse_json(char *json_str);
void print_json_object(const json_object *obj, int indent);
