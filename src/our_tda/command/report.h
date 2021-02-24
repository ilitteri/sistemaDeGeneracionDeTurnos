#include <stdlib.h>

typedef struct Report Report;

/* Crea la estructura */
Report *report_create(const char *min, const char *max);

/*  Devuelve la cota inferior del recorrido del informe.
*   Pre: la estructura fué creada.
*   Pos: nombre o letra que marca límite inferior de la búsqueda, si es vacío,
*       se guarda "`" como máximo caracter (uno más grande que la "z" en ASCII).
*/
char *report_min(const Report *report);

/*  Devuelve la cota superior del recorrido del informe.
*   Pre: la estructura fué creada.
*   Pos: nombre o letra que marca límite superior de la búsqueda, si es vacío,
*       se guarda "{" como máximo caracter (uno más grande que la "z" en ASCII).
*/
char *report_max(const Report *report);

/*  Informa la cantidad de doctores incluídos en el informe 
*   Pre: la estructura fué creada.
*/
size_t report_get_count(const Report *report);

/*  Incrementa en uno el contador interno de la estructura.
*   Pre: la estructura fué creada.
*/
void report_count_increment(Report *report);

/* Destruye la estructura */
void report_destroy(Report *report);