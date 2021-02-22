#ifndef LOAD_STRUCTURE_FUNCTIONS
#define LOAD_STRUCTURE_FUNCTIONS

#include <stdlib.h>

#include "lista.h"

#include "hash_patients.h"
#include "hash_turns.h"
#include "bst_doctors.h"

HashTurns *load_hash_turns(lista_t *doctor_csv_lines);

HashPatients *load_patients(lista_t *patient_csv_lines);

BSTDoctors *load_doctors(lista_t *doctor_csv_lines);

#endif