#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* Tipos de función para comparar claves y destruir datos. */
typedef int (*abb_comparar_clave_t)(const char *, const char *);
typedef void (*abb_destruir_dato_t)(void *);

/* Los structs deben llamarse 'abb' y 'abb_iter'. */

typedef struct abb_nodo abb_nodo_t;
typedef struct abb abb_t;

struct abb_iter;
typedef struct abb_iter abb_iter_t;

/* Primitivas del mapa. */

/* Crea el abb */
abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *abb, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *abb, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue inicializada
 */
void *abb_obtener(const abb_t *abb, const char *clave);


/* Determina si clave pertenece o no al abb.
 * Pre: La estructura abb fue inicializada
 */
bool abb_pertenece(const abb_t *abb, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: La estructura abb fue inicializada
 */
size_t abb_cantidad(abb_t *abb);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * wrapper que va borrando cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *abb);

/* Implementa el iterador interno, que recorre el arbol in-order.
   "visitar" es una función de callback que recibe la clave, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario).
 */
void abb_in_order(abb_t *abb,
                  bool visitar(const char *, void *, void *),
                  void *extra);

/* Primitivas del iterador externo. */

/* Crea iterador */
abb_iter_t *abb_iter_in_crear(const abb_t *abb);

/* Avanza el iterador 
* Pre: La estructura abb y el iterador fueron inicializados.
* Post: El iterador avanzó o no.
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Devuelve clave actual, esa clave no se puede modificar ni liberar. 
* Pre: La estructura abb y el iterador fueron inicializados.
* Post: Clave actual que corresponde a la posición del iterador o NULL si no hay.
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Comprueba si terminó la iteración. 
* Pre: La estructura abb y el iterador fueron inicializados.
* Post: Indicación de si iterador está al final del recorrido o no.
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador. 
* Pre: La estructura abb y el iterador fueron inicializados.
* Post: El iterador fue destruido.
*/
void abb_iter_in_destruir(abb_iter_t *iter);

#endif  // ABB_H