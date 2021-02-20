#include <stdlib.h>

#include "hash_patients.h"
#include "patient.h"

Hash_Patients *hash_patients_create(hash_destroy_patient destroy_patient)
{
    Hash_Patients *patients;

    if ((patients = hash_crear(destroy_patient)) == NULL)
    {
        return NULL;
    }

    return patients;
}

bool hash_patients_save(Hash_Patients *patients, Patient *patient)
{
    return hash_guardar(patients, patient_name(patient), patient);
}

bool hash_patients_exists(const Hash_Patients *patients, const char *name)
{
    return hash_pertenece(patients, name);
}

void hash_patients_destroy(Hash_Patients *patients)
{
    hash_destruir(patients);
}