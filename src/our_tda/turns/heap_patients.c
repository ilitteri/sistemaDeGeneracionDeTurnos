#include <stdlib.h>

#include "heap_patients.h"
#include "../patient/patient.h"


HeapPatients *heap_patients_create(HeapPatients_cmp cmp)
{
    HeapPatients *regular;

    if ((regular = heap_crear((cmp_func_t)cmp)) == NULL)
    {
        return NULL;
    }

    return regular;
}

void heap_patients_destroy(HeapPatients *turns, bool destroy)
{
    heap_destruir(turns, destroy ? (void (*)(void *))destroy_patient : NULL);
}

size_t heap_patients_count(const HeapPatients *turns)
{
    return heap_cantidad(turns);
}

bool heap_patients_enqueue(HeapPatients *turns, Patient *patient)
{
    return heap_encolar(turns, (void *)patient);
}

Patient *heap_patients_dequeue(HeapPatients *turns)
{
    return (Patient *)heap_desencolar(turns);
}