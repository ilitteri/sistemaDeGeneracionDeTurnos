#include <stdlib.h>

#include "bst_doctors.h"
#include "../../basic_tda/avl.h"
#include "doctor.h"
#include "../command/range.h"

BSTDoctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data)
{
    BSTDoctors *doctors;

    if ((doctors = avl_create(cmp, destroy_data)) == NULL)
    {
        return NULL;
    }

    return doctors;
}

bool bst_doctors_save_doctor(BSTDoctors *doctors, const char *doctor_name, Doctor *doctor)
{
    return avl_save(doctors, doctor_name, (void *)doctor);
}

Doctor *bst_doctors_get_doctor(const BSTDoctors *doctors, const char *doctor_name)
{
    return (Doctor *)avl_get(doctors, doctor_name);
}

size_t bst_doctors_count(BSTDoctors *doctors)
{
    return avl_count(doctors);
}

void bst_doctors_destroy(BSTDoctors *doctors)
{
    avl_destroy(doctors);
}

void bst_doctors_in_range(BSTDoctors *doctors, 
                        bool visit(const char *, void *, void *), 
                        Range *range)
{
    avl_in_range(doctors, visit, range, range_min(range), range_max(range));
}