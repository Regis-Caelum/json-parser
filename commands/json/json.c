#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "../../utility/utility.h"

static int validateJson(char *jsonStr)
{
    printf("File contents:\n%s\n", jsonStr);
    return 0;
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
