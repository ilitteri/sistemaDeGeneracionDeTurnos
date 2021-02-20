#include <stdio.h>        /* perror */
#include <errno.h>        /* errno */
#include <stdlib.h>       /* malloc, free, exit */

#include "messages.h"

int handle_command_error(int argc);
int handle_file_error(int argc, char *path);


int main(int argc, char **argv) 
{
    handle_command_error(argc);

    char *doctors_path = argv[1];
    char *patients_path = argv[2];

    FILE *doctors_file, *patients_file;

    doctors_file = fopen(doctors_path, "r"); 
    handle_file_error(errno, doctors_path);

    patients_file = fopen(doctors_path, "r"); 
    handle_file_error(errno, patients_path);

    fclose(doctors_file);
    fclose(patients_file);

    return 0; 
} 

int handle_command_error(int argc)
{
    if (argc != 3)
    {
        printf(ERR_PARAMS_COUNT);
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