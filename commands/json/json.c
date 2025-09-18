#include "json.h"

void json_init(json_object *json_obj)
{
    json_obj->length = 0;
    json_obj->entries = NULL;
}

void add_json_entry(json_object *json_obj, json_object_entry *json_obj_entry)
{
    if (json_obj->length == 0)
    {
        json_obj->entries = malloc(sizeof(json_object_entry *));
        if (!json_obj->entries)
        {
            fprintf(stderr, "Could not allocate the memory for the json entry");
            exit(1);
        }
        memcpy(json_obj->entries + json_obj->length * sizeof(json_object_entry *), json_obj_entry, sizeof(json_object_entry *));
        json_obj->length++;
        return;
    }

    json_obj->entries = realloc(json_obj->entries, sizeof(json_object_entry *));
    if (!json_obj->entries)
    {
        fprintf(stderr, "Could not allocate memory for the json entry");
        exit(1);
    }

    memcpy(json_obj->entries + (json_obj->length + 1) * sizeof(json_object_entry *), json_obj_entry, sizeof(json_object_entry *));
    json_obj->length++;
}

void json_free(json_object *json_obj)
{
    free(json_obj->entries);
    json_obj->entries = NULL;
    json_obj->length = 0;
}
