#include <stdlib.h>

#include "hash.h"
#include "patient.h"

typedef hash_t HashPatients;
typedef hash_destruir_dato_t hash_destroy_patient;

/* Crea la estructura */
HashPatients *hash_patients_create(hash_destroy_patient destroy_data);

/*  Guarda un paciente en la estrucutra
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: se guarda al paciente en la estructura.
*/
bool hash_patients_save(HashPatients *patients, Patient *patient);

/*  Informa si el paciente está registrado
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: el paciente está guardado o no.
*/
bool hash_patients_exists(const HashPatients *patients, const char *name);

/* Destruye la estrucura y sus datos en caso de existir remanentes */
void hash_patients_destroy(HashPatients *patients);