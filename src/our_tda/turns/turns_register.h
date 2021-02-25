#ifndef TURNS_REGISTER_H
#define TURNS_REGISTER_H

#include <stdlib.h>
#include <stdbool.h>

#include "../../basic_tda/hash.h"
#include "../../basic_tda/cola.h"
#include "queue_patients.h"
#include "heap_patients.h"
#include "../doctor/doctor.h"

/* Declaración del struct de la estructura*/

typedef struct Register TurnsRegister;

/* Funciones de la estructura */

typedef hash_destruir_dato_t turns_register_destroy_data;

/* Primitivas de la estructura */

/* Crea la estructura */
TurnsRegister *turns_register_create(turns_register_destroy_data destroy_queue_patients, 
                            turns_register_destroy_data destroy_heap_patients);

/*  Agrega un paciente a la cola de espera (dependiendo de la urgencia, va a
*   una u otra).
*   Pre: la estructura TurnsRegister fué creada.
*/
bool turns_register_add_turn(TurnsRegister *turns, char* urgency, char *specialty, Patient *patient);

/*  Agrega una especialidad a el diccionario de turnos urgentes y regulares.
*   Pre: la estructura TurnsRegister fué creada.
*/
bool turns_register_add_specialty(TurnsRegister *turns, char *specialty);

/*  Atiende al siguiente paciente en espera.
*   Pre: la estructura TurnsRegister fué creada, la especialidad existe, y el
*       doctor está especializado en ella.
*   Pos: datos del paciente desencolado.
*/
Patient *turns_register_attend_patient(TurnsRegister *turns, Doctor *doctor, char *specialty);

/*  Informa si existe una determinada especialidad.
*   Pre: la estructura TurnsRegister fué creada, la especialidad existe, y el doctor
*       está especializado en ella.
*   Pos: true si existe, false si no.
*/
bool turns_register_specialty_exists(TurnsRegister *turns, char *specialty);

/*  Informa la cantidad de pacientes en espera de una especialidad.
*   Pre: la estructura TurnsRegister fué creada.
*/
size_t turns_register_specialty_count(TurnsRegister *turns, char *specialty);

/*  Destruye la estructura */
void turns_register_destroy(TurnsRegister *turns);

#endif