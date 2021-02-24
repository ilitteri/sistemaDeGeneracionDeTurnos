#ifndef QUEUE_PATIENTS_H
#define QUEUE_PATIENTS_H

#include <stdlib.h>

#include "../patient/patient.h"

/* Struct */

typedef struct Queue QueuePatients;

/* Primitivas */

/* Crea la estructura */
QueuePatients *queue_patients_create();

/*  Destruye la estrucutura .
*   Pre: la estructura fue creada.
*/
void queue_patients_destroy(QueuePatients *patients, void (*patients_destroy)(void *));

/*  Informa si la cola se encuentra vacía (no modifica a la cola).
*   Pre: la estructura fue creada.
*/
bool queue_patients_is_empty(const QueuePatients *patients);

/*  Encola un paciente.
*   Pre: la estructura fue creada.
*/
bool queue_patients_enqueue(QueuePatients *patients, Patient *patient);

/*  Devuelve la información del primero en la cola, sin modificarla.
*   Pre: la estrucura fue creada.
*/
Patient *queue_patients_first(const QueuePatients *patients);

/*  Desencola un paciente.
*   Pre: la estructura fue creada.
*/
Patient *queue_patients_dequeue(QueuePatients *patients);

/*  Informa la cantidad de pacientes en espera.
*   Pre: la estructura fue creada.
*/
size_t queue_patients_count(QueuePatients *urgent);

#endif