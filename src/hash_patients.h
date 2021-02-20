#include <stdlib.h>

#include "hash.h"
#include "patient.h"

typedef hash_t Hash_Patients;
typedef hash_destruir_dato_t hash_destroy_patient;

/* Crea la estructura */
Hash_Patients *create_patients_hash(hash_destroy_patient destroy_data);

/*  Guarda un paciente en la estrucutra
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: se guarda al paciente en la estructura.
*/
bool save_patient(Hash_Patients *patients, const char *name, Patient *patient);

/*  Informa si el paciente está registrado
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: el paciente está guardado o no.
*/
bool patient_exists(const Hash_Patients *patients, const char *name);

/* Destruye la estrucura y sus datos en caso de existir remanentes */
void destroy_patients_hash(Hash_Patients *patients);