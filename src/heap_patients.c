#include <stdlib.h>

#include "heap_patients.h"
#include "patient.h"


HeapPatients *heap_patients_create(HeapPatients_cmp cmp)
{
    HeapPatients *regular = heap_crear(cmp);
    return regular;
}

void heap_patients_destroy(HeapPatients *turns, void (*patients_destroy) (Patient *p))
{
    heap_destruir(turns, patients_destroy);
}

size_t heap_patients_count(const HeapPatients *turns)
{
    return heap_cantidad(turns);
}

bool heap_patients_enqueue(const HeapPatients *turns, Patient *patient)
{
    return heap_encolar(turns, (void *)patient);
}

HeapPatients *heap_patients_dequeue(HeapPatients *turns)
{
    return (HeapPatients *)heap_desencolar(turns);
}