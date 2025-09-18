#include "json_command.h"
#include "../../utility/utility.h"

static int file_flag(char *argv[])
{
    const char *filename = argv[2];
    char *buffer = read_file(filename);
    if (!buffer)
    {
        return 1;
    }
    json_object *json_obj = parse_json(buffer);
    free(buffer);
    return 0;
}

static int json_process_arguments(int argc, char *argv[])
{
    char *flag = argv[1];
    switch (flag[1])
    {
    case 'f':
        return file_flag(argv);
    default:
        fprintf(stderr, "Unknown flag: %s\n", flag);
        fprintf(stderr, "Usage: %s -f <file-path>\n", argv[0]);
        return 1;
    }

    return 0;
}

command json_command = {
    .name = "json",
    .process_arguments = json_process_arguments};
