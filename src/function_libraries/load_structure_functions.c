#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../basic_tda/hash.h"
#include "../basic_tda/abb.h"
#include "../basic_tda/heap.h"
#include "../basic_tda/cola.h"
#include "../basic_tda/lista.h"
#include "../basic_tda/pila.h"

#include "../our_tda/patient/patient.h"
#include "../our_tda/doctor/doctor.h"
#include "../our_tda/patient/hash_patients.h"
#include "../our_tda/turns/heap_patients.h"
#include "../our_tda/turns/queue_patients.h"
#include "../our_tda/turns/hash_turns.h"
#include "../our_tda/doctor/bst_doctors.h"

void _queue_patients_destroy(void *queue)
{
    queue_patients_destroy((QueuePatients *)queue, NULL);
}

void _heap_patients_destroy(void *heap)
{
    heap_patients_destroy((HeapPatients *)heap, NULL);
}

HashTurns *load_hash_turns(lista_t *doctor_csv_lines)
{
    HashTurns *turns;

    if ((turns  = hash_turns_create(_queue_patients_destroy, _heap_patients_destroy)) == NULL)
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

void patient_destroy(void *patient)
{
    destroy_patient((Patient *)patient);
}

HashPatients *load_patients(lista_t *patient_csv_lines)
{
    HashPatients *patient_register;

    if  ((patient_register = hash_patients_create(patient_destroy)) == NULL)
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