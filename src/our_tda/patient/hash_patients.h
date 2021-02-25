#ifndef HASH_PATIENTS_H
#define HASH_PATIENTS_H

#include <stdlib.h>
#include <stdbool.h>

#include "../../basic_tda/hash.h"
#include "patient.h"

/* Declaración del struct de la estructura*/

typedef hash_t HashPatients;

/* Funciones de la estructura */

typedef hash_destruir_dato_t hash_destroy_patient;

/* Primitivas de la estructura */

/* Crea la estructura */
HashPatients *hash_patients_create(hash_destroy_patient patient_destroy);

/*  Guarda un paciente en la estrucutra
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: devuelve si el paciente se registró correctamente.
*/
bool hash_patients_save(HashPatients *patients, Patient *patient);

/*  Informa si el paciente está registrado
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: devuelve si el paciente estaba registrado anteriormente.
*/
bool hash_patients_exists(const HashPatients *patients, const char *name);

/*  Devuelve un paciente
*   Pre: la estrucutra fue creada, se registró al paciente.
*/
Patient *hash_patients_get(const HashPatients *patients, const char *name);

/* Destruye la estrucura y sus datos en caso de existir remanentes */
void hash_patients_destroy(HashPatients *patients);

#endif