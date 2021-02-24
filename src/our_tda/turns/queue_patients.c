#include <stdlib.h>

#include "../../basic_tda/cola.h"
#include "../patient/patient.h"
#include "queue_patients.h"

typedef struct Queue
{
    cola_t *patients;
    size_t count;
} QueuePatients;

QueuePatients *queue_patients_create()
{
    QueuePatients *urgent;
    if ((urgent = malloc(sizeof(QueuePatients))) == NULL)
    {
        return NULL;
    }

    if ((urgent->patients = cola_crear()) == NULL)
    {
        free(urgent);
        return NULL;
    }

    urgent->count = 0;

    return urgent;
}

void queue_patients_destroy(QueuePatients *urgent, void (*patients_destroy)(void *))
{
    cola_destruir(urgent->patients, patients_destroy);
    free(urgent);
}

bool queue_patients_is_empty(const QueuePatients *urgent)
{
    return cola_esta_vacia(urgent->patients);
}

bool queue_patients_enqueue(QueuePatients *urgent, Patient *patient)
{  
    if (!cola_encolar(urgent->patients, (void *)patient))
    {
        return false;
    }
    
    urgent->count++;
    return true;
}

Patient *queue_patients_first(const QueuePatients *urgent)
{
    return (Patient *)cola_ver_primero(urgent->patients);
}

Patient *queue_patients_dequeue(QueuePatients *urgent)
{
    Patient* patient = (Patient *)cola_desencolar(urgent->patients);

    if (patient == NULL)
    {
        return NULL;
    }

    urgent->count--;
    return patient;
}

size_t queue_patients_count(QueuePatients *urgent)
{
    return urgent->count;
}