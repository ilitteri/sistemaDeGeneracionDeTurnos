#include <stdlib.h>

#include "abb.h"
#include "doctor.h"

/* Tipos de función para comparar claves y destruir datos. */
typedef abb_comparar_clave_t bst_key_cmp;
typedef abb_destruir_dato_t bst_doctor_destroy;

/* Struct */
typedef abb_t BST_Doctors;

/* Primitivas de BST_Doctors */

/* Crea la estructura */
BST_Doctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data);

/* Guarda un doctor en la estrucutra
*  Pre: la estrucutra fue creada, se registró al doctor.
*  Pos: se guarda al doctor en la estructura.
*/
bool bst_save_doctor(BST_Doctors *doctors, const char *doctor_name, Doctor *doctor);

/* Devuelve el struct doctor correspondiente al nombre de este.
*  Pre: la estrucutra fue creada, se registró al doctor y se guardp en la estructura.
*  Pos: se devulve el doctor correspondiente.
*/
Doctor *bst_get_doctor(const BST_Doctors *doctors, const char *doctor_name);

/* Destruye la estructura y sus datos si bst_doctor_destroy es distinto de NULL */
void bst_doctors_destroy(BST_Doctors *doctors);