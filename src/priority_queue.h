#include <stdlib.h>

#include "cola.h"
#include "patient.h"

typedef cola_t PriorityQueue;

/* Crea la estrucutra */
PriorityQueue *priority_queue_create();

/*  Destruye la estrucutura 
*   Pre: la estructura fue creada.
*/
void priority_queue_destroy(PriorityQueue *queue, void (*destroy_patient) (Patient *patient));

/*  Encola un paciente
*   Pre: la estructura fue creada.
*/
bool enqueue_patient(PriorityQueue *queue, Patient *patient);

/*  Desencola un paciente
*   Pre: la estructura fue creada.
*/
Patient *dequeue_patient(PriorityQueue *queue);