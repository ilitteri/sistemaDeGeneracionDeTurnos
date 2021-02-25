#ifndef HEAP_PATIENTS_H
#define HEAP_PATIENTS_H

#include <stdlib.h>
#include <stdbool.h>

#include "../../basic_tda/heap.h"
#include "../patient/patient.h"

/* Declaración del struct de la estructura*/

typedef heap_t HeapPatients;

/* Funciones de la estructura */

typedef cmp_func_t HeapPatients_cmp;

/* Primitivas de la estructura */

/*  Crea la estrucutra */
HeapPatients *heap_patients_create(HeapPatients_cmp cmp);

/*  Destruye la estrucutra, y si su función de destrucción no es NULL, también
*   sus datos.
*   Pre: la estructura fué creada.
*   Pos: se libera la memoria pedida para la estructura.
*/
void heap_patients_destroy(HeapPatients *turns, void (*patient_destroy) (void *));

/*  Informa la cantidad de pacientes en espera (no modifica la estructura).
*   Pre: la estructura fue creada.
*/
size_t heap_patients_count(const HeapPatients *turns);

/*  Encola un paciente.
*   Pre: la estructura fué creada.
*   Pos: devuelve si el paciente se encoló correctamente.
*/
bool heap_patients_enqueue(HeapPatients *turns, Patient *patient);

/*  Desencola un paciente.
*   Pre: la estructura fué creada.
*   Pos: devuelve el paciente desencolado.
*/  
Patient *heap_patients_dequeue(HeapPatients *turns);

#endif