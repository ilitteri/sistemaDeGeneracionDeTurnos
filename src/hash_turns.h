#include <stdlib.h>

#include "hash.h"
#include "cola.h"
#include "queue_patients.h"
#include "heap_patients.h"

/* Struct */

typedef struct Hash HashTurns;

/* Primitivas de la estructura */

/* Crea la estructura */
HashTurns *hash_turns_create();

/*  Agrega un paciente a la cola de espera (dependiendo de la urgencia, va a
*   una u otra).
*   Pre: la estructura HashTurns fué creada.
*/
bool hash_turns_add_turn(HashTurns *turns, Priority urgency, char *specialty, Patient *patient);

/*  Agrega una especialidad a el diccionario de turnos urgentes y regulares.
*   Pre: la estructura HashTurns fué creada.
*/
bool hash_turns_add_specialty(HashTurns *turns, char *specialty);

/*  Atiende al siguiente paciente en espera.
*   Pre: la estructura HashTurns fué creada, la especialidad existe, y el
*       doctor está especializado en ella.
*   Pos: datos del paciente desencolado.
*/
Patient *hash_turns_attend_patient(HashTurns *turns, Doctor *doctor, char *specialty);

/*  Informa si existe una determinada especialidad.
*   Pre: la estructura HashTurns fué creada, la especialidad existe, y el doctor
*       está especializado en ella.
*   Pos: true si existe, false si no.
*/
bool hash_turns_specialty_exists(HashTurns *turns, char *specialty);

/*  Informa la cantidad de pacientes en espera de una especialidad.
*   Pre: la estructura HashTurns fué creada.
*/
size_t hash_turns_specialty_count(HashTurns *turns, Priority urgency, char *specialty);

/*  Destruye la estructura */
void hash_turns_destroy(HashTurns *turns);