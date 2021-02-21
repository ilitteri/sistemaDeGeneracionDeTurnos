#include <stdlib.h>

#include "bst_doctors.h"
#include "doctor.h"

BSTDoctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data)
{
    BSTDoctors *doctors;

    if ((doctors = abb_crear(cmp, destroy_data)) == NULL)
    {
        return NULL;
    }

    return doctors;
}

bool bst_doctors_save_doctor(BSTDoctors *doctors, const char *doctor_name, Doctor *doctor)
{
    return abb_guardar(doctors, doctor_name, doctor);
}

Doctor *bst_doctors_get_doctor(const BSTDoctors *doctors, const char *doctor_name)
{
    return abb_obtener(doctors, doctor_name);
}

void bst_doctors_destroy(BSTDoctors *doctors)
{
    abb_destruir(doctors);
}