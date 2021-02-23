#include <stdlib.h>
#include <string.h>

#include "doctor.h"

typedef struct Doctor
{
    char *name;
    char *specialty;
    size_t attended_patiens;
} Doctor;

Doctor *doctor_check_in(char *name, char *specialty)
{
    Doctor *doctor;

    if ((doctor = malloc(sizeof(Doctor))) == NULL)
    {
        return NULL;
    }

    char *name_copy, specialty_copy;
    if ((name_copy = malloc(strlen(name) + 1)) == NULL)
    {
        free(doctor);
        return NULL;
    }

    if ((specialty_copy = malloc(strlen(specialty) + 1)) == NULL)
    {
        free(name_copy);
        free(doctor);
        return NULL;
    }

    strcpy(name_copy, name);
    strcpy(specialty_copy, specialty);

    doctor->name = name_copy;
    doctor->specialty = specialty_copy;
    doctor->attended_patiens = 0;

    return doctor;
}

char *doctor_name(const Doctor *doctor)
{
    return doctor->name;
}

char *doctor_specialty(const Doctor *doctor)
{
    return doctor->specialty;
}

size_t doctor_attended_patients(const Doctor *doctor)
{
    return doctor->attended_patiens;
}

void doctor_attend_patient(Doctor *doctor)
{
    doctor->attended_patiens++;
}

void doctor_destroy(Doctor *doctor)
{
    free(doctor->name);
    free(doctor->specialty);
    free(doctor);
}