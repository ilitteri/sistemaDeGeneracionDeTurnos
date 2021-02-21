#include <stdlib.h>

#include "cola.h"
#include "patient.h"
#include "queue_patients.h"

typedef struct Queue
{
    cola_t patients;
    size_t cant;
} QueuePatients;

QueuePatients *queue_patients_create()
{
    QueuePatients *urgent;
    if ((urgent = cola_crear()) == NULL)
    {
        return NULL;
    }

    if ((urgent->patients = cola_crear()) == NULL)
    {
        free(urgent);
        return NULL;
    }

    urgent->cant = 0;

    return urgent;
}

void queue_patients_destroy(QueuePatients *urgent, void (*patients_destroy)(Patient *))
{
    cola_destruir(urgent->patients, patients_destroy);
}

bool queue_patients_is_empty(const QueuePatients *urgent)
{
    return cola_esta_vacia((const cola_t *) urgent->patients);
}

bool queue_patients_enqueue(QueuePatients *urgent, Patient *patient)
{  
    return cola_encolar(urgent->patients, (void *)patient);
}

Patient *queue_patients_first(const QueuePatients *urgent)
{
    return (Patient *)cola_ver_primero((const cola_t *)patients);
}

Patient *queue_patients_dequeue(QueuePatients *patients)
{
    return (Patient *)cola_desencolar((cola_t *)patients);
}