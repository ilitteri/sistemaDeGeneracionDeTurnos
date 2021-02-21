#include <stdlib.h>

#include "patient.h"
#include "queue_urgents.h"
#include "queue_patients.h"

typedef struct Queue
{
    QueuePatients patients;
    size_t cant;
} QueueUrgents;

QueueUrgents *queue_urgents_create()
{
    QueueUrgents *urgents;
    if ((urgents = malloc(sizeof(QueueUrgents))) == NULL)
    {
        return NULL;
    }

    if ((urgents->patients = queue_patients_create()) == NULL)
    {
        free(urgents);
        return NULL;
    }

    urgents->cant = 0;

    return urgents;
}

void queue_urgents_destroy(QueueUrgents *urgents)
{
    queue_patients_destroy(urgents->patients);
}

bool queue_urgents_enqueue(QueueUrgents *urgents, Patient *patient)
{
    return queue_patients_enqueue(urgents->patients, patient);
}

Patient *queue_urgents_dequeue(QueueUrgents *urgents)
{
    return queue_patients_dequeue(urgents->patients);
}