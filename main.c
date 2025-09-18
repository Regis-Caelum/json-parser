#include <stdio.h>
#include <string.h>
#include "command.h"
#include "commands/json/json_command.h"

extern command json_command;

command *commands[] = {&json_command, NULL};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("No command given\n");
        return 1;
    }

    for (int i = 0; commands[i] != NULL; i++)
    {
        if (strcmp(argv[1], commands[i]->name) == 0)
        {
            return commands[i]->process_arguments(argc - 1, &argv[1]);
        }
    }

    printf("Unknown command: %s\n", argv[1]);
    return 1;
}
