#include <stdlib.h>

#include "cola.h"
#include "patient.h"

typedef cola_t PriorityQueue;

PriorityQueue *priority_queue_create();

void priority_queue_destroy(PriorityQueue *queue, void (*destroy_patient) (Patient *patient));

bool enqueue_patient(PriorityQueue *queue, Patient *patient);

Patient *dequeue_patient(PriorityQueue *queue);