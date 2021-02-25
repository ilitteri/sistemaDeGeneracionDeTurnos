#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "lista.h"

/* ******************************************************************
 *                    DEFINICION DE ESTRUCTURAS
 * *****************************************************************/

typedef struct nodo {
    void *dato;
    struct nodo *proximo;
} nodo_t;

struct lista {
    nodo_t *primero;
    nodo_t *ultimo;
    size_t largo;
};

struct lista_iter{
	nodo_t **indirecto_actual; // Puntero a puntero, lo uso para referirme indirectamente al actual, a traves de un puntero
    lista_t *lista;
};

/* ******************************************************************
 *                    IMPLEMENTACION DE LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
    lista_t *lista; // O(1)

    if ((lista = malloc(sizeof(lista_t))) == NULL) // O(1)
        return NULL; // O(1)
    
    lista->primero = NULL; // O(1)
    lista->ultimo = NULL; // O(1)
    lista->largo = 0; // O(1)

    return lista; // O(1)
} // O(1) * 7

bool lista_esta_vacia(const lista_t *lista) {
    return !lista->largo; // O(1)
} // O(1)

nodo_t *crear_nodo(void *dato) {
    nodo_t *nodo_nuevo; // O(1)
    
    if ((nodo_nuevo = malloc(sizeof(nodo_t))) == NULL)
        return NULL; // O(1)
    
    nodo_nuevo->dato = dato; // O(1)
    nodo_nuevo->proximo = NULL;

    return nodo_nuevo; // O(1)
} // O(1) * 4 

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo_nuevo; // O(1)
    
    if ((nodo_nuevo = crear_nodo(dato)) == NULL) // O(1)
        return false; // O(1)
    
    if (lista_esta_vacia(lista))
        lista->ultimo = nodo_nuevo;

    nodo_nuevo->proximo = lista->primero;
    lista->primero = nodo_nuevo;
    lista->largo++;

    return true; // O(1)
} // O(1) * 7

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    if (lista_esta_vacia(lista)) // O(1)
        return lista_insertar_primero(lista, dato); // O(1)
    
    nodo_t *nodo_nuevo; // O(1)
    
    if ((nodo_nuevo = crear_nodo(dato)) == NULL) // O(1)
        return false; // O(1)

    lista->ultimo->proximo = nodo_nuevo; // O(1)
    lista->ultimo = nodo_nuevo; // O(1)
    lista->largo++; // O(1)

    return true; // O(1)
} // O(1) * 9

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) // O(1)
        return NULL; // O(1)
    
    nodo_t *nodoAuxiliar = lista->primero; // O(1)
    void *dato = nodoAuxiliar->dato; // O(1)

    lista->primero = lista->primero->proximo; // O(1)
    free(nodoAuxiliar); // O(1)
    lista->largo--; // O(1)

    if (lista->largo == 0)
        lista->ultimo = NULL;

    return dato; // O(1)
} // O(1) * 8

void *lista_ver_primero(const lista_t *lista) {
    return lista_esta_vacia(lista) ? NULL : lista->primero->dato; // O(1)
} // O(1)

void *lista_ver_ultimo(const lista_t *lista) {
    return lista_esta_vacia(lista) ? NULL : lista->ultimo->dato; // O(1)
} // O(1)

size_t lista_largo(const lista_t *lista) {
    return lista->largo; // O(1)
} // O(1)

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {

    while (!lista_esta_vacia(lista)) {
        void *dato = lista_borrar_primero(lista); // O(1)
        if (destruir_dato != NULL) // O(1)
            destruir_dato(dato); // O(1)
    } // O(n) (n = lista->largo) y no O(2n) porque el compilador saca el if afuera y lo ahce una vez

    free(lista); // O(1)
} // O(n) + O(1) * 2

/* ******************************************************************
 *                    IMPLEMENTACION DE ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter; // O(1)

    if ((iter = malloc(sizeof(lista_iter_t))) == NULL) // O(1)
        return NULL; // O(1)
    
    // Guardo en el puntero indirecto actual al referencia al miembro "primero"
    // de la lista, para cuando lo desreferencie obtener el nodo al que
    // apunta el primero.
    iter->indirecto_actual = &lista->primero; // O(1)
    iter->lista = lista; // O(1) 

    return iter; // O(1)
} // O(1) * 6

bool lista_iter_avanzar(lista_iter_t *iter) {
    // Si el iterador esta al final, no puedo avanzar mas.
    if (lista_iter_al_final(iter))
        return false;

    // El indirecto actual ahora es la referencia al proximo del nodo que
    // apunto indirectamente.
    iter->indirecto_actual = &(*iter->indirecto_actual)->proximo;

    return true; // O(1)
} // O(1) * 3

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    // Si la lista esta vacia o si el nodo al que apunto indirectamente es NULL
    // no hay actual para devolver, sino devuelvo el dato del nodo,
    // desreferenciando el puntero indirecto actual.
    return lista_iter_al_final(iter) ? NULL : (*iter->indirecto_actual)->dato;
} // O(1)

bool lista_iter_al_final(const lista_iter_t *iter) {
    // Si la lista esta vacia o si el nodo al que apunto indirectamente es NULL
    // entonces estoy en el final, ya sea porque la lista esta vacia, o porque
    // estoy parado al final directamente.
    return lista_esta_vacia(iter->lista) || *iter->indirecto_actual == NULL;
} // O(1)

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter); // O(1)
} // O(1)

/* ******************************************************************
 *               PRIMITIVAS DE LISTAS JUNTO CON ITERADOR
 * *****************************************************************/

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {

    nodo_t *nodo_nuevo; // O(1)

    if ((nodo_nuevo = crear_nodo(dato)) == NULL) // O(1)
        return false; // O(1)

    // El proximo del nodo nuevo es el nodo actual al que apunto indirectamente.
    nodo_nuevo->proximo = *iter->indirecto_actual; // O(1)
    // El nodo actual al que apunto indirectamente ahora es el nodo nuevo.
    *iter->indirecto_actual = nodo_nuevo; // O(1)

    // Si el proximo del nodo nuevo es NULL, el nodo nuevo es el ultimo.
    if (nodo_nuevo->proximo == NULL) // O(1)
        iter->lista->ultimo = nodo_nuevo; // O(1)

    iter->lista->largo++; // O(1)

    return true; // O(1)
} // O(1) * 9

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) // O(1)
        return NULL; // O(1)

    // Guardo el nodo actual para luego liberar la memoria que ocupa.
    nodo_t *nodo_a_matar = *iter->indirecto_actual;
    // Guardo el dato del nodo a matar para devolverlo.
    void *dato = nodo_a_matar->dato;
    // El actual pasa a ser el proximo.
    *iter->indirecto_actual = nodo_a_matar->proximo;
    
    iter->lista->largo--; // O(1)
    // Si borro y la lista queda vacia o si quedo en el final, hay que actualizar
    // el ultimo de la lista.
    if (lista_iter_al_final(iter)) {
        // Si quedo vacia entonces el ultimo es NULL
        if (lista_esta_vacia(iter->lista)) // O(1)
            iter->lista->ultimo = NULL; // O(1)

        else {
            // En esta linea, el offsetof es una macro que devuelve el offset del
            // miembro proximo dentro del tipo nodo_t en bytes, offset que le resto
            // al puntero iter->indirecto_actual (apunta al miembro proximo del nodo 
            // anterior), al realizar esta resta, ahora apunta a la base del nodo 
            // anterior. Castee la operacion a uintptr_t para poder realizar la
            // operacion correctamente.
            nodo_t *anterior = (nodo_t *)((uintptr_t)iter->indirecto_actual - offsetof(nodo_t, proximo)); 
            iter->lista->ultimo = anterior;
        }
    }

    free(nodo_a_matar); // O(1)

    return dato; // O(1)
}

/* ******************************************************************
 *                    IMPLEMENTACION DE ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra) {
    if (visitar == NULL || lista->largo == 0) // O(1)
        return; // O(1)

    for (nodo_t *nodoAuxiliar = lista->primero; 
        nodoAuxiliar != NULL && visitar(nodoAuxiliar->dato, extra); 
        nodoAuxiliar = nodoAuxiliar->proximo); // O(n) (aca no estoy seguro con lo de la funcion visitar)
} // O(n) ?