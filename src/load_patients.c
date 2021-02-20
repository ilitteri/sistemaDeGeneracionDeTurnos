#include <stdlib.h>

#include "patient.h"
#include "hash_patients.h"
#include "lista.h"

Hash_Patients *load_patients(lista_t *patient_csv_lines)
{
    Hash_Patients *patient_register;

    if  ((patient_register = create_patients_hash(destroy_patient)) == NULL)
    {
        return NULL;
    }

    lista_iter_t *list_iter;

    if ((list_iter = lista_iter_crear(patient_csv_lines)) == NULL)
    {
        destroy_patients_hash(patient_register);
        return NULL;
    }

    bool ok = true;
    while (!lista_iter_al_final(list_iter))
    {
        Patient *patient;

        if ((patient = patient_check_in(lista_iter_ver_actual(list_iter))) == NULL)
        {
            lista_iter_destruir(list_iter);
            destroy_patients_hash(patient_register);
            return NULL;
        }

        ok &= save_patient(patient_register, patient);

        lista_iter_avanzar(list_iter);
    }

    lista_iter_destruir(list_iter);

    if (!ok)
    {
        destroy_patients_hash(patient_register);
        return NULL;
    }

    return patient_register;
}