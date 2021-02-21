#include <stdlib.h>

#include "hash_patients.h"
#include "patient.h"

HashPatients *hash_patients_create(hash_destroy_patient destroy_patient)
{
    HashPatients *patients;

    if ((patients = hash_crear(destroy_patient)) == NULL)
    {
        return NULL;
    }

    return patients;
}

bool hash_patients_save(HashPatients *patients, Patient *patient)
{
    return hash_guardar(patients, patient_name(patient), patient);
}

bool hash_patients_exists(const HashPatients *patients, const char *name)
{
    return hash_pertenece(patients, name);
}

void hash_patients_destroy(HashPatients *patients)
{
    hash_destruir(patients);
}