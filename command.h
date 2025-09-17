#pragma once

typedef struct
{
    const char *name;
    int (*process_arguments)(int argc, char *argv[]);
} command;
