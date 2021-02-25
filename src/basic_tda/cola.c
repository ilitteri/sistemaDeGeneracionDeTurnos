#include <stdlib.h>
#include <stdbool.h>

#include "cola.h"

typedef struct nodo {
    void *dato;
    struct nodo *proximo;
} nodo_t;

struct cola {
    nodo_t *primero;
    nodo_t *ultimo;
};

cola_t *cola_crear(void) {
    cola_t *cola;

    if ((cola = malloc(sizeof(nodo_t))) == NULL)
        return NULL;
    
    cola->primero = NULL;
    cola->ultimo = NULL;

    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    void *dato;

    while ((dato = cola_desencolar(cola)) != NULL)
        if (destruir_dato != NULL)
            destruir_dato(dato);

    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_t *nodo;

    if ((nodo = malloc(sizeof(nodo_t))) == NULL)
        return false;
    
    nodo->dato = valor;
    nodo->proximo = NULL;

    if (cola_esta_vacia(cola))
        cola->primero = nodo;
    else
        cola->ultimo->proximo = nodo;
        
    cola->ultimo = nodo;

    return true;
}

void *cola_ver_primero(const cola_t *cola) {
    return cola_esta_vacia(cola) ? NULL : cola->primero->dato;
}

void *cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola))
        return NULL;
    
    void *dato = cola->primero->dato;
    nodo_t *nodoProximo = cola->primero->proximo;

    free(cola->primero);

    if (cola->primero == cola->ultimo) 
        cola->ultimo = NULL;

    cola->primero = nodoProximo;

    return dato;
}