#include <stdlib.h>

#include "patient.h"

typedef struct
{
    char *name;
    size_t entry_year;
} Patient;

Patient *patient_check_in(char *name, size_t entry_year)
{
    Patient *patient;

    if ((patient = malloc(sizeof(Patient))) == NULL)
    {
        return NULL;
    }

    patient->name = name;
    patient->entry_year = entry_year;

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
    // free(name)
    free(patient);
}