#include <stdlib.h>
#include <string.h>
#include "doctor.h"
#include "bst_doctors.h"
#include "lista.h"

BST_Doctors *load_doctors(lista_t *doctor_csv_lines)
{
    BST_Doctors *doctor_register;

    if ((doctor_register = bst_doctors_create(strcmp, destroy_doctor)) == NULL)
    {
        return NULL;
    }

    lista_iter_t *doctor_list_iter;

    if ((doctor_list_iter = lista_iter_crear(doctor_csv_lines)) == NULL)
    {
        bst_doctors_destroy(doctor_register);
        return NULL;
    }

    bool ok = true;
    while(!lista_iter_al_final(doctor_list_iter)){
        Doctor* doctor;
        char** doctor_data = (char**) lista_iter_ver_actual(doctor_list_iter);

        char* name_copy = malloc(strlen(doctor_data[0]) + 1);
        char* specialty_copy = malloc(strlen(doctor_data[1]) + 1);
        strcpy(name_copy, doctor_data[0]);
        strcpy(specialty_copy, doctor_data[1]);

        if ((doctor = doctor_check_in(name_copy, specialty_copy)) == NULL)
        {
            lista_iter_destruir(doctor_list_iter);
            bst_doctors_destroy(doctor_register);
            return NULL;
        }

        ok &= bst_save_doctor(doctor_register, strcpy(malloc(strlen(doctor_data[0]) + 1), doctor_data[0]), doctor);
        lista_iter_avanzar(doctor_list_iter);

    }
    lista_iter_destruir(doctor_list_iter);

    if (!ok)
    {
        bst_doctors_destroy(doctor_register);
        return NULL;
    }

    return doctor_register;
}