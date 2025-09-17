#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "json.h"
#include "../../utility/utility.h"

static bool isValidJsonNumber(const char *s, const char **endptr)
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

static int validateJson(char *jsonStr)
{
    int res = 0;
    char *q;
    bool in_string = false;
    Stack stack;
    stack_init(&stack, sizeof(char *));

    for (char *p = jsonStr; *p; p++)
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
                if (!isValidJsonNumber(p, &endptr))
                {
                    printf("Invalid number at position %ld\n", p - jsonStr);
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
                stack_push(&stack, p);
                break;
            case '}':
                if (!stack_empty(&stack))
                {
                    char c = '\0';
                    stack_pop(&stack, &c);
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
                if (!stack_empty(&stack))
                {
                    char c = '\0';
                    stack_pop(&stack, &c);
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
                while (q > jsonStr && isspace((unsigned char)*q))
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
                printf("Unquoted string detected: '%c' at pos %ld\n", *p, p - jsonStr);
                res = 1;
                break;
            }

            if (res)
            {
                stack_free(&stack);
                return res;
            }
        }
    }

    stack_free(&stack);
    return res;
}

static int parseJson(char *jsonStr, JsonObject *jsonObject)
{
    return validateJson(jsonStr);
}

static int fileFlag(char *argv[])
{
    const char *filename = argv[2];
    char *buffer = read_file(filename);
    if (!buffer)
    {
        return 1;
    }
    JsonObject *jsonObj = NULL;
    parseJson(buffer, jsonObj);
    free(buffer);
    return 0;
}

static int jsonProcessArguments(int argc, char *argv[])
{
    char *flag = argv[1];
    switch (flag[1])
    {
    case 'f':
        return fileFlag(argv);
    default:
        fprintf(stderr, "Unknown flag: %s\n", flag);
        fprintf(stderr, "Usage: %s -f <file-path>\n", argv[0]);
        return 1;
    }

    return 0;
}

Command JsonCommand = {
    .name = "json",
    .processArguments = jsonProcessArguments};
