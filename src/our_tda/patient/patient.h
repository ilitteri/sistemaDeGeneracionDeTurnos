#ifndef PATIENT_H
#define PATIENT_H

#include <stdlib.h>

/* Declaración del struct de la estructura*/

typedef struct Patient Patient;

/* Primitivas de la estructura */

/* Registra un paciente */
Patient *patient_check_in(char *name, size_t year);

/*  Obtiene el nombre de el paciente (no modifica al paciente)
*   Pre: el paciente fué registrado.
*   Pos: nombre del paciente.
*/
char *patient_name(const Patient *patient);

/*  Obtiene el anio de entrada del paciente (no modifica al paciente)
*   Pre: el paciente fué registrado.
*   Pos: anio de registro del paciente.
*/
size_t patient_entry_year(const Patient *patient);

/* Destruye los datos del paciente */
void destroy_patient(Patient *patient);

#endif