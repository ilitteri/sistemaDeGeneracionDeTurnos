#include <stdlib.h>

#include "doctor.h"

typedef struct Doctor{
    char* name;
    char* specialty;
    int attended_patiens;
} Doctor;

Doctor *doctor_check_in(char *name, char *specialty)
{
    Doctor *doctor;

    if ((doctor = malloc(sizeof(Doctor))) == NULL){

        return NULL;
    }

    doctor->name = name;
    doctor->specialty = specialty;
    doctor->attended_patiens = 0;

    return doctor;
}

char *get_name(const Doctor *doctor)
{
    return doctor->name;
}

char *get_specialty(const Doctor *doctor)
{
    return doctor->specialty;
}

int get_attended_patients(const Doctor *doctor)
{
    return doctor->attended_patiens;
}

void attend_patient(Doctor *doctor)
{
    doctor->attended_patiens++;
}

void destroy_doctor(Doctor *doctor)
{
    free(doctor->name);
    free(doctor->specialty);
    free(doctor);
}