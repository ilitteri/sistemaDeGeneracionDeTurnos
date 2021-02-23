#include "pila.h"

#include <stdlib.h>

#define CAPACIDAD 8
#define FACTOR_AGRANDAR 2
#define FACTOR_REDUCIR 0.5
#define LIMITE_REDUCIR 0.25

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    unsigned cantidad;   // Cantidad de elementos almacenados.
    unsigned capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t *pila;

    if ((pila = malloc(sizeof(pila_t))) == NULL)
        return NULL;

    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD;

    if ((pila->datos = malloc(sizeof(void *) * pila->capacidad)) == NULL) {
        free(pila);
        return NULL;
    }

    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_redimensionar(pila_t *pila, unsigned capacidad) {
    void *temp = realloc(pila->datos, sizeof(void *) * (capacidad));
    if (temp == NULL)
        return false;

    pila->capacidad = capacidad;
    pila->datos = temp;

    return true;
}

bool pila_esta_vacia(const pila_t *pila) {
    return !(pila->cantidad);
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        if (!pila_redimensionar(pila, (unsigned)(pila->capacidad * FACTOR_AGRANDAR)))
            return false;
    }

    pila->datos[pila->cantidad++] = valor;

    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    return pila->cantidad ? pila->datos[pila->cantidad-1] : NULL;
}

void *pila_desapilar(pila_t *pila) {
    if (pila->cantidad == (unsigned)(pila->capacidad * LIMITE_REDUCIR)) {
        if (!pila_redimensionar(pila, (unsigned)(pila->capacidad * FACTOR_REDUCIR)))
            return NULL;
    }
    
    return pila->cantidad ? pila->datos[--pila->cantidad] : NULL;
}
