#include <stdlib.h>

#include "patient.h"
#include "priority_queue.h"

PriorityQueue *priority_queue_create()
{
    PriorityQueue *queue;
    return (queue = cola_crear()) == NULL ? NULL : queue;
}

void priority_queue_destroy(PriorityQueue *queue, void (*destroy_patient) (Patient *patient))
{
    cola_destruir(queue, destroy_patient);
}

bool enqueue_patient(PriorityQueue *queue, Patient *patient)
{
    return cola_encolar(queue, patient);
}

Patient *dequeue_patient(PriorityQueue *queue)
{
    return (Patient) cola_desencolar(queue);
}