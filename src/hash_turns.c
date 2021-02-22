#include <stdlib.h>
#incldue <stdbool.h>

#include "hash.h"
#include "hash_turns.h"
#include "queue_patients.h"
#include "heap_patients.h"

typedef enum Priority
{
    URGENT,
    REGULAR
} Priority;

typedef struct Hash
{
    hash_t *urgent;
    hash_t *regular;
} HashTurns;

static bool add_urgent_turn(hash_t *urgent, char *specialty, char *patient_name);
static bool add_regular_turn(hash_t *regular, char *specialty, char *patient_name);
static bool add_urgent_specialty(hash_t *urgent, char* specialty);
static bool add_regular_specialty(hash_t *regular, char* specialty);

HashTurns *hash_turns_create()
{
    HashTurns *turns;

    if ((turns = hash_crear(hash_destruir)) == NULL)
    {
        return NULL;
    }

    if ((turns->urgent = hash_crear(queue_patients_destroy)) == NULL)
    {
        free(turns);
        return NULL;
    }

    if ((turns->regular = hash_crear(heap_patients_destroy)) == NULL)
    {
        free(turns->urgent);
        free(turns);
        return NULL;
    }

    return turns;
}

static bool add_urgent_turn(hash_t *urgent, char *specialty, char *patient_name)
{
    
}

static bool add_regular_turn(hash_t *regular, char *specialty, char *patient_name)
{

}

bool hash_turns_add_turn(HashTurns *turns, Priority urgency, char *specialty, char *patient_name)
{
    if (urgency == URGENT)
    {
        return add_urgent_turn(turns->urgent, specialty, patient_name);
    }

    else if (urgency == REGULAR)
    {
        return add_regular_turn(turns->regular, specialty, patient_name);
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
        return n == NULL ? -1 : 1;
    }

    size_t n_year = patient_entry_year((Patient *)n);
    size_t m_year = patient_entry_year((Patient *)m);

    return n_year == m_year ? 0 : n_year > m_year ? 1 : -1; 
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

    else
    {
        return false;
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
        queue_patients_destroy(hash_borrar(turns->urgent, specialty));
        heap_patients_destroy(hash_borrar(turns->regular, specialty));
        return false;
    }

    return true;
}

void hash_turns_destroy(HashTurns *turns)
{
    hash_destruir(turns->urgent);
    hash_destruir(turns->regular);
    free(turns);
}
