#include <stdlib.h>
#include <string.h>

#include "patient.h"

typedef struct Patient
{
    char *name;
    size_t entry_year;
} Patient;

static char *copy_name(char *original_name)
{   
    char *name = malloc(strlen(original_name) + 1);
    if (name == NULL)
    {
        return NULL;
    }
    strcpy(name, original_name);
    return name;
}

Patient *patient_check_in(char *name, size_t year)
{
    Patient *patient;

    if ((patient = malloc(sizeof(Patient))) == NULL)
    {
        return NULL;
    }

    char *name_cpy;
    if ((name_cpy = copy_name(name)) == NULL)
    {
        free(patient);
        return NULL;
    }

    patient->name = name_cpy;
    patient->entry_year = year;

    return patient;
}

char *patient_name(const Patient *patient)
{
    return patient->name;
}

size_t patient_entry_year(const Patient *patient)
{
    return patient->entry_year;
}

void destroy_patient(Patient *patient)
{
    free(patient->name);
    free(patient);
}