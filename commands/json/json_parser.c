#include "json_parser.h"

static json_value *parse_value(xarray *tokens, size_t *pos);
static json_object *parse_object(xarray *tokens, size_t *pos);
static json_array *parse_array(xarray *tokens, size_t *pos);

static bool is_valid_json_number(const char *s, const char **endptr)
{
    const char *p = s;

    if (*p == '-')
        p++;

    if (*p == '0')
    {
        p++;
        if (isdigit((unsigned char)*p))
            return false;
    }
    else if (isdigit((unsigned char)*p))
    {
        while (isdigit((unsigned char)*p))
            p++;
    }
    else
    {
        return false;
    }

    if (*p == '.')
    {
        p++;
        if (!isdigit((unsigned char)*p))
            return false;
        while (isdigit((unsigned char)*p))
            p++;
    }

    if (*p == 'e' || *p == 'E')
    {
        p++;
        if (*p == '+' || *p == '-')
            p++;
        if (!isdigit((unsigned char)*p))
            return false;
        while (isdigit((unsigned char)*p))
            p++;
    }

    if (endptr)
        *endptr = p;
    return true;
}

static int validate_json(char *json_str)
{
    int res = 0;
    char *q;
    bool in_string = false;
    stack token_stack;
    stack_init(&token_stack, sizeof(char *));

    for (char *p = json_str; *p; p++)
    {
        if (*p == '"')
        {
            in_string = !in_string;
            continue;
        }

        if (!in_string)
        {
            if (strncmp("false", p, 5) == 0)
            {
                p += 4;
                continue;
            }
            else if (strncmp("true", p, 4) == 0)
            {
                p += 3;
                continue;
            }
            else if (strncmp("null", p, 4) == 0)
            {
                p += 3;
                continue;
            }

            if (*p == '-' || isdigit((unsigned char)*p))
            {
                const char *endptr;
                if (!is_valid_json_number(p, &endptr))
                {
                    printf("Invalid number at position %ld\n", p - json_str);
                    res = 1;
                }
                else
                {
                    p = (char *)endptr - 1;
                    continue;
                }
            }

            switch (*p)
            {
            case '[':
            case '{':
                stack_push(&token_stack, p);
                break;
            case '}':
                if (!stack_empty(&token_stack))
                {
                    char c = '\0';
                    stack_pop(&token_stack, &c);
                    if (c != '{')
                    {
                        printf("Unbalanced braces\n");
                        res = 1;
                    }
                }
                else
                {
                    res = 1;
                }
                break;
            case ']':
                if (!stack_empty(&token_stack))
                {
                    char c = '\0';
                    stack_pop(&token_stack, &c);
                    if (c != '[')
                    {
                        printf("Unbalanced brackets\n");
                        res = 1;
                    }
                }
                else
                {
                    res = 1;
                }
                break;
            case ',':
                q = p + 1;
                while (*q && isspace((unsigned char)*q))
                    q++;
                if (*q == '}' || *q == ']')
                {
                    printf("Trailing comma detected\n");
                    res = 1;
                }
                break;
            case ':':
                q = p - 1;
                while (q > json_str && isspace((unsigned char)*q))
                    q--;
                if (*q != '"')
                {
                    printf("Unquoted key detected\n");
                    res = 1;
                }
                break;
            case '\t':
            case '\n':
            case ' ':
                break;
            default:
                printf("Unquoted string detected: '%c' at pos %ld\n", *p, p - json_str);
                res = 1;
                break;
            }

            if (res)
            {
                stack_free(&token_stack);
                return res;
            }
        }
    }

    stack_free(&token_stack);
    return res;
}

static xarray *tokenize_json_string(char *json_str)
{
    xarray *arr = malloc(sizeof(xarray));
    xarray_init(arr, sizeof(char *));

    while (*json_str)
    {
        if (isspace((unsigned char)*json_str))
        {
            json_str++;
            continue;
        }

        char *token = NULL;

        if (strchr("{}[]:,", *json_str))
        {
            token = strndup(json_str, 1);
            json_str++;
        }
        else if (*json_str == '"')
        {
            char *start = ++json_str;
            while (*json_str && *json_str != '"')
            {
                if (*json_str == '\\' && *(json_str + 1))
                    json_str++;
                json_str++;
            }
            size_t len = json_str - start;
            token = strndup(start, len);
            if (*json_str == '"')
                json_str++;
        }
        else
        {
            char *start = json_str;
            while (*json_str && !isspace((unsigned char)*json_str) && !strchr("{}[]:,", *json_str))
                json_str++;
            size_t len = json_str - start;
            token = strndup(start, len);
        }

        if (token)
            xarray_push_back(arr, &token);
    }

    return arr;
}

static char *get_token(xarray *tokens, size_t pos)
{
    char *tok;
    memcpy(&tok, (char *)tokens->data + pos * tokens->elem_size, sizeof(char *));
    return tok;
}

static json_value *make_value(json_type type)
{
    json_value *v = malloc(sizeof(json_value));
    v->type = type;
    v->u = malloc(sizeof(*(v->u)));
    return v;
}

static json_object *parse_object(xarray *tokens, size_t *pos)
{
    json_object *obj = malloc(sizeof(json_object));
    obj->entries = NULL;
    obj->length = 0;

    (*pos)++;

    while (*pos < tokens->size)
    {
        char *tok = get_token(tokens, *pos);
        if (strcmp(tok, "}") == 0)
        {
            (*pos)++;
            break;
        }

        char *key = get_token(tokens, *pos);
        (*pos)++;

        if (strcmp(get_token(tokens, *pos), ":") != 0)
        {
            fprintf(stderr, "Expected ':' after key\n");
            exit(EXIT_FAILURE);
        }
        (*pos)++;

        json_value *val = parse_value(tokens, pos);

        obj->entries = realloc(obj->entries, sizeof(json_object_entry *) * (obj->length + 1));
        json_object_entry *entry = malloc(sizeof(json_object_entry));
        entry->key = strdup(key);
        entry->value = val;
        obj->entries[obj->length++] = entry;

        if (strcmp(get_token(tokens, *pos), ",") == 0)
        {
            (*pos)++;
            continue;
        }
    }
    return obj;
}

static json_array *parse_array(xarray *tokens, size_t *pos)
{
    json_array *arr = malloc(sizeof(json_array));
    arr->items = NULL;
    arr->length = 0;

    (*pos)++;

    while (*pos < tokens->size)
    {
        char *tok = get_token(tokens, *pos);
        if (strcmp(tok, "]") == 0)
        {
            (*pos)++;
            break;
        }

        json_value *val = parse_value(tokens, pos);
        arr->items = realloc(arr->items, sizeof(json_value *) * (arr->length + 1));
        arr->items[arr->length++] = val;

        if (strcmp(get_token(tokens, *pos), ",") == 0)
        {
            (*pos)++;
            continue;
        }
    }
    return arr;
}

static json_value *parse_value(xarray *tokens, size_t *pos)
{
    char *tok = get_token(tokens, *pos);

    if (strcmp(tok, "{") == 0)
    {
        json_value *v = make_value(JSON_OBJECT);
        v->u->object = parse_object(tokens, pos);
        return v;
    }
    else if (strcmp(tok, "[") == 0)
    {
        json_value *v = make_value(JSON_ARRAY);
        v->u->array = parse_array(tokens, pos);
        return v;
    }
    else if (strcmp(tok, "true") == 0 || strcmp(tok, "false") == 0)
    {
        json_value *v = make_value(JSON_BOOL);
        v->u->boolean = (strcmp(tok, "true") == 0);
        (*pos)++;
        return v;
    }
    else if (strcmp(tok, "null") == 0)
    {
        json_value *v = make_value(JSON_NULL);
        (*pos)++;
        return v;
    }
    else if (tok[0] == '"' || isalpha((unsigned char)tok[0]))
    {
        json_value *v = make_value(JSON_STRING);
        v->u->string = strdup(tok);
        (*pos)++;
        return v;
    }
    else
    {
        json_value *v = make_value(JSON_NUMBER);
        v->u->number = atof(tok);
        (*pos)++;
        return v;
    }
}

json_object *parse_json(char *json_str)
{
    int is_json_valid = validate_json(json_str);
    if (is_json_valid != 0 || !strlen(json_str))
        return NULL;

    xarray *arr = tokenize_json_string(json_str);
    if (!arr)
    {
        perror("could not tokenize the json string");
        exit(EXIT_FAILURE);
    }

    size_t pos = 0;
    json_value *root = parse_value(arr, &pos);

    if (root->type != JSON_OBJECT)
    {
        fprintf(stderr, "Root must be a JSON object\n");
        exit(EXIT_FAILURE);
    }

    return root->u->object;
}

static void print_json_value(const json_value *value, int indent);

static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        putchar(' ');
    }
}

void print_json_object(const json_object *obj, int indent)
{
    printf("{\n");
    for (size_t i = 0; i < obj->length; i++)
    {
        print_indent(indent + 2);
        printf("\"%s\": ", obj->entries[i]->key);
        print_json_value(obj->entries[i]->value, indent + 2);
        if (i < obj->length - 1)
        {
            printf(",");
        }
        printf("\n");
    }
    print_indent(indent);
    printf("}");
    printf("\n\n");
}

static void print_json_array(const json_array *arr, int indent)
{
    printf("[\n");
    for (size_t i = 0; i < arr->length; i++)
    {
        print_indent(indent + 2);
        print_json_value(arr->items[i], indent + 2);
        if (i < arr->length - 1)
        {
            printf(",");
        }
        printf("\n");
    }
    print_indent(indent);
    printf("]");
}

static void print_json_value(const json_value *value, int indent)
{
    switch (value->type)
    {
    case JSON_NULL:
        printf("null");
        break;
    case JSON_BOOL:
        printf(value->u->boolean ? "true" : "false");
        break;
    case JSON_NUMBER:
        printf("%g", value->u->number);
        break;
    case JSON_STRING:
        printf("\"%s\"", value->u->string);
        break;
    case JSON_ARRAY:
        print_json_array(value->u->array, indent);
        break;
    case JSON_OBJECT:
        print_json_object(value->u->object, indent);
        break;
    }
}
