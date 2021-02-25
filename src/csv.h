#ifndef  __CSV__H_
#define  __CSV__H_

#include "basic_tda/lista.h"
#include "function_libraries/strutil.h"

/**
Haciendo uso de strutil (split) lee un archivo csv línea a línea.

Se devuelve una lista con todos los elementos construidos con split. 
Es decir una lista de arrays (char**) donde cada array es una linea del csv.
devuelve NULL en caso de error al crear la lista.
**/
lista_t* csv_create(FILE* csv_file);

/**
 * Destruye la lista de arrays y el contenido de estos.
 **/
void csv_destroy(lista_t* list);

#endif