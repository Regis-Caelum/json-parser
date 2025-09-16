#pragma once

typedef struct
{
    const char *name;
    int (*processArguments)(int argc, char *argv[]);
} Command;
