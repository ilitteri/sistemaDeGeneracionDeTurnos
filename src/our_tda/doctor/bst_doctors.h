#ifndef BST_DOCTORS_H
#define BST_DOCTORS_H

#include <stdlib.h>

#include "../../basic_tda/abb.h"
#include "doctor.h"
#include "../command/range.h"

/* Funciones de la estructura */

typedef avl_cmp_key bst_key_cmp;
typedef avl_destroy_data bst_doctor_destroy;

/* Struct */
typedef AVL BSTDoctors;

/* Primitivas de BSTDoctors */

/* Crea la estructura */
BSTDoctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data);

/* Guarda un doctor en la estrucutra
*  Pre: la estrucutra fue creada, se registró al doctor.
*  Pos: se guarda al doctor en la estructura.
*/
bool bst_doctors_save_doctor(BSTDoctors *doctors, const char *doctor_name, Doctor *doctor);

/* Devuelve el struct doctor correspondiente al nombre de este.
*  Pre: la estrucutra fue creada, se registró al doctor y se guardó en la estructura.
*  Pos: se devulve el doctor correspondiente.
*/
Doctor *bst_doctors_get_doctor(const BSTDoctors *doctors, const char *doctor_name);

/* Devuelve el struct doctor correspondiente al nombre de este.
*  Pre: la estrucutra fue creada, se registró al doctor y se guardó en la estructura.
*  Pos: se devulve la cantidad de doctores registrados.
*/
size_t bst_doctors_count(BSTDoctors *doctors);

/* Destruye la estructura y sus datos si bst_doctor_destroy es distinto de NULL */
void bst_doctors_destroy(BSTDoctors *doctors);

/* ITERADOR INTERNO */

/* Implementa el iterador interno en rango, que recorre el arbol in-order.
   "visitar" es una función de callback que recibe la clave, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario), el rango se recibe como arreglo de arreglos (strv), el iterador
   itera en el rango dado.
 */
void bst_doctors_in_range(BSTDoctors *doctors,
                          bool visit(const char *, void *, void *),
                          Range *range);

#endif