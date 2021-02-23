#ifndef LOAD_STRUCTURE_FUNCTIONS
#define LOAD_STRUCTURE_FUNCTIONS

#include <stdlib.h>

#include "../basic_tda/lista.h"

#include "../our_tda/patient/hash_patients.h"
#include "../our_tda/turns/hash_turns.h"
#include "../our_tda/doctor/bst_doctors.h"

/* 
*   Crea el esqueleto del diccionario de turnos urgentes y regulares utilizando
*   las especialidades que se encuentran en los datos de los doctores.
*/
HashTurns *load_hash_turns(lista_t *doctor_csv_lines);

/*  
*   Con los datos de los pacientes, crea estructuras para almacenarlos, y
*   almacena esas estructuras en un diccionario.
*/
HashPatients *load_patients(lista_t *patient_csv_lines);

/*  
*   Con los datos de los doctores, crea estructuras para almacenarlos, y
*   almacena esas estructuras en un árbol binario.
*/
BSTDoctors *load_doctors(lista_t *doctor_csv_lines);

#endif