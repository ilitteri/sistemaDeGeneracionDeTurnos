#include <stdlib.h>

#include "heap.h"
#include "patient.h"

typedef heap_t HeapPatients;

typedef cmp_func_t HeapPatients_cmp;

HeapPatients *heap_patients_create(HeapPatients_cmp cmp);

void heap_patients_destroy(HeapPatients *turns, void (*patient_destroy) (Patient *p));

size_t heap_patients_count(const HeapPatients *turns);

bool heap_patients_enqueue(const HeapPatients *turns);

HeapPatients *heap_patients_dequeue(HeapPatients *turns); 