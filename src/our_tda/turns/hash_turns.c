#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../../basic_tda/hash.h"
#include "hash_turns.h"
#include "queue_patients.h"
#include "heap_patients.h"
#include "../doctor/doctor.h"
#include "../patient/patient.h"

#define URGENT "URGENTE"
#define REGULAR "REGULAR"

typedef struct Hash
{
    hash_t *urgent;
    hash_t *regular;
} HashTurns;

static bool add_urgent_turn(hash_t *urgent, char *specialty, Patient *patient);
static bool add_regular_turn(hash_t *regular, char *specialty, Patient *patient);
static bool add_urgent_specialty(hash_t *urgent, char* specialty);
static bool add_regular_specialty(hash_t *regular, char* specialty);

void _destroy_patient(void *patient)
{
    destroy_patient((Patient *)patient);
}

void _queue_patients_destroy(void *queue)
{
    queue_patients_destroy((QueuePatients *)queue, NULL);
}

void _heap_patients_destroy(void *heap)
{
    heap_patients_destroy((HeapPatients *)heap, NULL);
}

HashTurns *hash_turns_create(hash_turns_destroy_data destroy_data)
{
    HashTurns *turns;

    if ((turns = malloc(sizeof(HashTurns))) == NULL)
    {
        return NULL;
    }

    if ((turns->urgent = hash_crear(_queue_patients_destroy)) == NULL)
    {
        free(turns);
        return NULL;
    }

    if ((turns->regular = hash_crear(_heap_patients_destroy)) == NULL)
    {
        free(turns->urgent);
        free(turns);
        return NULL;
    }

    return turns;
}

static bool add_urgent_turn(hash_t *urgent, char *specialty, Patient *patient)
{
    return queue_patients_enqueue((QueuePatients *)hash_obtener(urgent, specialty), patient);
}

static bool add_regular_turn(hash_t *regular, char *specialty, Patient *patient)
{
    return heap_patients_enqueue((HeapPatients *)hash_obtener(regular, specialty), patient);
}

bool hash_turns_add_turn(HashTurns *turns, char* urgency, char *specialty, Patient *patient)
{
    if (strcmp(urgency, URGENT) == 0)
    {
        return add_urgent_turn(turns->urgent, specialty, patient);
    }

    else if (strcmp(urgency, REGULAR) == 0)
    {
        return add_regular_turn(turns->regular, specialty, patient);
    }

    return false;
}

static bool add_urgent_specialty(hash_t *urgent, char* specialty)
{
    if (!hash_pertenece(urgent, specialty))
    {
        QueuePatients *waiting_patients;
        if ((waiting_patients = queue_patients_create()) == NULL)
        {
            return false;
        }
        hash_guardar(urgent, specialty, waiting_patients);
    }
    return true;
}

int patient_entry_year_cmp(const void *n, const void *m)
{
    if (n == NULL && m == NULL)
    {
        return 0;
    }

    else if (n == NULL || m == NULL)
    {
        return n == NULL ? 1 : -1;
    }

    size_t n_year = patient_entry_year((Patient *)n);
    size_t m_year = patient_entry_year((Patient *)m);

    return n_year == m_year ? 0 : n_year > m_year ? -1 : 1; 
}

static bool add_regular_specialty(hash_t *regular, char* specialty)
{
    if (!hash_pertenece(regular, specialty))
    {
        HeapPatients *waiting_patients;
        if ((waiting_patients = heap_patients_create(patient_entry_year_cmp)) == NULL)
        {
            return false;
        }
        hash_guardar(regular, specialty, waiting_patients);
    }
    
    return true;
}

bool hash_turns_add_specialty(HashTurns *turns, char *specialty)
{
    bool ok = true;
    ok &= add_regular_specialty(turns->regular, specialty);
    ok &= add_urgent_specialty(turns->urgent, specialty);

    if (!ok)
    {
        queue_patients_destroy(hash_borrar(turns->urgent, specialty), _destroy_patient);
        heap_patients_destroy(hash_borrar(turns->regular, specialty), _destroy_patient);
        return false;
    }

    return true;
}

Patient *hash_turns_attend_patient(HashTurns *turns, Doctor *doctor, char *specialty)
{
    if (queue_patients_count(hash_obtener(turns->urgent, specialty)))
    {
        doctor_attend_patient(doctor);
        return queue_patients_dequeue(hash_obtener(turns->urgent, specialty));
    }

    else if (heap_patients_count(hash_obtener(turns->regular, specialty)))
    {
        doctor_attend_patient(doctor);
        return heap_patients_dequeue(hash_obtener(turns->regular, specialty));
    }

    return NULL;
}

bool hash_turns_specialty_exists(HashTurns *turns, char *specialty)
{
    return hash_pertenece(turns->urgent, specialty);
}

size_t hash_turns_specialty_count(HashTurns *turns, char *specialty)
{
    return queue_patients_count(hash_obtener(turns->urgent, specialty)) +
            heap_patients_count(hash_obtener(turns->regular, specialty));
}

void hash_turns_destroy(HashTurns *turns)
{
    hash_destruir(turns->urgent);
    hash_destruir(turns->regular);
    free(turns);
}
