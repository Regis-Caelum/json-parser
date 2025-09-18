#include "json_parser.h"

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

json_object *parse_json(char *json_str)
{
    int is_json_valid = validate_json(json_str);
    if (is_json_valid != 0)
    {
        perror("invalid json");
        exit(EXIT_FAILURE);
    }

    xarray *arr = tokenize_json_string(json_str);
    if (!arr)
    {
        perror("could not tokenize the json string");
        exit(EXIT_FAILURE);
    }

    json_object *json_obj = NULL;
    json_init(json_obj);

    return 0;
}
