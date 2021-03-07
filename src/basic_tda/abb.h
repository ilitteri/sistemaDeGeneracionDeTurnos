#ifndef AVL_H
#define AVL_H

#include <stdbool.h>
#include <stdlib.h>

/* Tipos de función para cmp keys y destroy datos. */
typedef int (*avl_cmp_key)(const char *, const char *);
typedef void (*avl_destroy_data)(void *);

/* Los structs deben llamarse 'avl' y 'avl_iter'. */

typedef struct Node Node;
typedef struct AVL AVL;

struct AVL_Iter;
typedef struct AVL_Iter AVL_Iter;

/* Primitivas del mapa. */

/* Crea el avl */
AVL *abb_crear(avl_cmp_key cmp, avl_destroy_data destroy_data);
bool abb_guardar(AVL *avl, const char *key, void *dato);

/* Borra un elemento del avl y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura avl fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(AVL *avl, const char *key);

/* Obtiene el valor de un elemento del avl, si la key no se encuentra
 * devuelve NULL.
 * Pre: La estructura avl fue inicializada
 */
void *abb_obtener(const AVL *avl, const char *key);

/* Determina si key pertenece o no al avl.
 * Pre: La estructura avl fue inicializada
 */
bool abb_pertenece(const AVL *avl, const char *key);

/* Devuelve la cantidad de elementos del avl.
 * Pre: La estructura avl fue inicializada
 */
size_t abb_cantidad(AVL *avl);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * wrapper que va borrando cada par (key, dato).
 * Pre: La estructura avl fue inicializada
 * Post: La estructura avl fue destruida
 */
void abb_destruir(AVL *avl);

/* Implementa el iterador interno en rango, que recorre el arbol in-order.
   "func" es una función de callback que recibe la key, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario), el rango se recibe como arreglo de arreglos (strv), el iterador
   itera en el rango dado.
 */
void avl_in_range(AVL *avl,
                  bool func(const char *, void *, void *),
                  void *extra,
                  char *min, char *max);

/* Implementa el iterador interno, que recorre el arbol in-order.
   "func" es una función de callback que recibe la key, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario).
 */
void abb_in_order(AVL *avl,
                  bool func(const char *, void *, void *),
                  void *extra);

/* Primitivas del iterador externo. */

/* Crea iterador */
AVL_Iter *abb_iter_in_crear(const AVL *avl);

/* Avanza el iterador 
* Pre: La estructura avl y el iterador fueron inicializados.
* Post: El iterador avanzó o no.
*/
bool abb_iter_in_avanzar(AVL_Iter *iter);

/* Devuelve key actual, esa key no se puede modificar ni liberar. 
* Pre: La estructura avl y el iterador fueron inicializados.
* Post: key actual que corresponde a la posición del iterador o NULL si no hay.
*/
const char *abb_iter_in_ver_actual(const AVL_Iter *iter);

/* Comprueba si terminó la iteración. 
* Pre: La estructura avl y el iterador fueron inicializados.
* Post: Indicación de si iterador está al final del recorrido o no.
*/
bool abb_iter_in_al_final(const AVL_Iter *iter);

/* Destruye el iterador. 
* Pre: La estructura avl y el iterador fueron inicializados.
* Post: El iterador fue destruido.
*/
void abb_iter_in_destruir(AVL_Iter *iter);

#endif // AVL_H