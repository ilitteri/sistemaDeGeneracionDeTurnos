#include <stdlib.h>

#include "bst_doctors.h"
#include "doctor.h"

BST_Doctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data)
{
    BST_Doctors *doctors;

    if ((doctors = abb_crear(cmp, destroy_data)) == NULL)
    {
        return NULL;
    }

    return doctors;
}

bool bst_save_doctor(BST_Doctors *doctors, const char *doctor_name, Doctor *doctor)
{
    return abb_guardar(doctors, doctor_name, doctor);
}

Doctor *bst_get_doctor(const BST_Doctors *doctors, const char *doctor_name)
{
    return abb_obtener(doctors, doctor_name);
}

void bst_doctors_destroy(BST_Doctors *doctors)
{
    abb_destruir(doctors);
}