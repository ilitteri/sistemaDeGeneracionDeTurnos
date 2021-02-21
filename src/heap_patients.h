#include <stdlib.h>

#include "heap.h"
#include "patient.h"

typedef heap_t HeapPatients;

typedef cmp_func_t HeapPatients_cmp;

Heap_Turns *heap_regulars_create(HeapPatients_cmp cmp);

void heap_regulars_destroy(Heap_Turns *turns, void (*patient_destroy) (Patient *p));

size_t heap_regulars_count(const Heap_Turns *turns);

bool heap_regulars_enqueue(const Heap_Turns *turns);

Heap_Turns *heap_regulars_dequeue(Heap_Turns *turns); 