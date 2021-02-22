#ifndef COMMAND_FUNCTIONS
#define COMMAND_FUNCTIONS

#include "hash_turns.h"
#include "hash_patients.h"
#include "bst_doctors.h"

/*
*   Se recibe un nombre de paciente y el nombre de una especialidad, y el 
*   sistema le añade a la lista de espera de la especialidad correspondiente.
*/
void make_appointment(HashTurns *turns, HashPatients *patients, char **parameters);

/*
*   Se recibe el nombre de le doctore que quedó libre, y este atiende al 
*   siguiente paciente urgente (por orden de llegada). Si no hubiera ningún 
*   paciente urgente, atiende al siguiente paciente con mayor antigüedad como 
*   paciente en la clínica.
*/
void attend_patient(HashTurns *turns, BSTDoctors *doctors, char **parameters);

/*   
*   El sistema imprime la lista de doctores en orden alfabético, junto con su 
*   especialidad y el número de pacientes que atendieron desde que arrancó el 
*   sistema. Opcionalmente, se puede especificar el rango (alfabético) de 
*   doctores sobre los que se desean informes.
*   Pre: se creó y llenó de datos un registro de doctores.
*/
void generate_report(BSTDoctors *doctors, char **parameters);

#endif