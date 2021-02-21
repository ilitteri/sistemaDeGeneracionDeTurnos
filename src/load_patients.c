#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patient.h"
#include "hash_patients.h"
#include "lista.h"

static char *extract_name(char *original_name)
{   
    char *name = malloc(strlen(original_name) + 1);
    if (name == NULL)
    {
        return NULL;
    }
    strcpy(name, original_name);
    return name;
}

static void extract_year(char *original_year, size_t *year)
{
    sscanf(original_year, "%zu", year);
}

HashPatients *load_patients(lista_t *patient_csv_lines)
{
    HashPatients *patient_register;

    if  ((patient_register = hash_patients_create(destroy_patient)) == NULL)
    {
        return NULL;
    }

    lista_iter_t *list_iter;

    if ((list_iter = lista_iter_crear(patient_csv_lines)) == NULL)
    {
        hash_patients_destroy(patient_register);
        return NULL;
    }

    bool ok = true;
    while (!lista_iter_al_final(list_iter))
    {
        Patient *patient;

        char **patient_data = lista_iter_ver_actual(list_iter);

        size_t year;
        extract_year(patient_data[1], &year);

        if ((patient = patient_check_in(patient_data[0], year)) == NULL)
        {
            lista_iter_destruir(list_iter);
            hash_patients_destroy(patient_register);
            return NULL;
        }

        ok &= hash_patients_save(patient_register, patient);

        lista_iter_avanzar(list_iter);
    }

    lista_iter_destruir(list_iter);

    if (!ok)
    {
        hash_patients_destroy(patient_register);
        return NULL;
    }

    return patient_register;
}