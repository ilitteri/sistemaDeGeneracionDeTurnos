#ifndef RANGE_H
#define RANGE_H

#include <stdlib.h>

typedef struct Range Range;

/* Crea la estructura */
Range *range_create(const char *min, const char *max);

/*  Devuelve la cota inferior del recorrido del informe.
*   Pre: la estructura fué creada.
*   Pos: nombre o letra que marca límite inferior de la búsqueda, si es vacío,
*       se guarda "A" como mínimo caracter.
*/
char *range_min(const Range *range);

/*  Devuelve la cota superior del recorrido del informe.
*   Pre: la estructura fué creada.
*   Pos: nombre o letra que marca límite superior de la búsqueda, si es vacío,
*       se guarda "z" como máximo caracter.
*/
char *range_max(const Range *range);

/*  Informa la cantidad de doctores incluídos en el informe 
*   Pre: la estructura fué creada.
*/
size_t range_get_count(const Range *range);

/*  Incrementa en uno el contador interno de la estructura.
*   Pre: la estructura fué creada.
*/
void range_count_increment(Range *range);

/* Destruye la estructura */
void range_destroy(Range *range);

#endif