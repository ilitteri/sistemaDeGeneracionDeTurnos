#include <stdlib.h>

#include "cola.h"
#include "patient.h"

typedef struct
{
    cola_t patients;
    size_t cant;
} QueueUrgents;

/* Crea la estrucutra */
QueueUrgents *queue_urgents_create();

/*  Destruye la estrucutura 
*   Pre: la estructura fue creada.
*/
void queue_urgents_destroy(QueueUrgents *queue, void (*destroy_patient) (Patient *patient));

/*  Encola un paciente
*   Pre: la estructura fue creada.
*/
bool queue_urgents_enqueue(QueueUrgents *queue, Patient *patient);

/*  Desencola un paciente
*   Pre: la estructura fue creada.
*/
Patient *queue_urgents_dequeue(QueueUrgents *queue);