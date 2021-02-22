#include <stdlib.h>

#include "hash.h"
#include "abb.h"
#include "heap.h"
#include "cola.h"
#include "lista.h"
#include "pila.h"

#include "patient.h"
#include "doctor.h"
#include "hash_patients.h"
#include "heap_patients.h"
#include "queue_patients.h"
#include "hash_turns.h"
#include "bst_doctors.h"

HashTurns *load_hash_turns(lista_t *doctor_csv_lines)
{
    HashTurns *turns;

    if ((turns  = hash_turns_create()) == NULL)
    {
        return NULL;
    }

    lista_iter_t *doctor_list_iter;

    if ((doctor_list_iter = lista_iter_crear(doctor_csv_lines)) == NULL)
    {
        hash_turns_destroy(turns);
        return NULL;
    }

    while (!lista_iter_al_final(doctor_list_iter))
    {
        char **doctor_data = (char **)lista_iter_ver_actual(doctor_list_iter);

        if (hash_turns_add_specialty(turns, doctor_data[1]) == false)
        {
            lista_iter_destruir(doctor_list_iter);
            hash_turns_destroy(turns);
            return NULL;
        }

        lista_iter_avanzar(doctor_list_iter);
    }
    lista_iter_destruir(doctor_list_iter);

    return turns;
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

        size_t year = (size_t)atoi(patient_data[1]);

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

static void _doctor_destroy(void* doctor)
{
    doctor_destroy((Doctor *)doctor);
}

BSTDoctors *load_doctors(lista_t *doctor_csv_lines)
{
    BSTDoctors *doctor_register;

    if ((doctor_register = bst_doctors_create(strcmp, _doctor_destroy)) == NULL)
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
    while (!lista_iter_al_final(doctor_list_iter))
    {
        Doctor *doctor;
        char **doctor_data = (char **)lista_iter_ver_actual(doctor_list_iter);

        if ((doctor = doctor_check_in(doctor_data[0], doctor_data[1])) == NULL)
        {
            lista_iter_destruir(doctor_list_iter);
            bst_doctors_destroy(doctor_register);
            return NULL;
        }

        ok &= bst_doctors_save_doctor(doctor_register, doctor_data[0], doctor);
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