#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */

#include "messages.h"

int handle_command_error(int argc)
{
    if (argc != 3)
    {
        perror("S");
        printf(ERR_CMD_PARAMS_COUNT);
        exit(EXIT_FAILURE);
    }
    return 0;
}

int handle_file_error(int errnum, char *path)
{
    if (errnum == 2)
    {
        printf(ERR_FILE, path);
        exit(EXIT_FAILURE);
    }
    return 0;
}