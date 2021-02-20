#include <stdlib.h>

#include "hash_patients.h"
#include "patient.h"

Hash_Patients *create_patients_hash(hash_destroy_patient destroy_patient)
{
    Hash_Patients *patients;

    if ((patients = hash_crear(destroy_patient)) == NULL)
    {
        return NULL;
    }

    return patients;
}

bool save_patient(Hash_Patients *patients, const char *name, Patient *patient)
{
    return hash_guardar(patients, patient_name(patient), patient);
}

bool patient_exists(const Hash_Patients *patients, const char *name)
{
    return hash_pertenece(patients, name);
}

void destroy_patients_hash(Hash_Patients *patients)
{
    hash_destruir(patients);
}