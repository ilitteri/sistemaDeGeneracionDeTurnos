#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "abb.h"
#include "pila.h"

/* Definicion de Structs */

typedef struct abb_nodo
{
    struct abb_nodo *izq;
    struct abb_nodo *der;
    char *clave;
    void *dato;
} abb_nodo_t;

typedef struct abb
{
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cantidad;
} abb_t;

typedef struct abb_iter
{
    pila_t *estados;
} abb_iter_t;

/* Definicion de enums */

typedef enum hijos
{
    SIN_RELACION = 0,
    HIJO_IZQ = 1,
    HIJO_DER = 2
} hijos;

/* Definicion de funciones auxiliares */

// static bool recorrer(abb_nodo_t *actual, const char *clave, void **dato, abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato, motivos_recorrido motivo_recorrido);
static abb_nodo_t *abb_nodo_crear(const char *clave, void *dato);
static abb_nodo_t *buscar_y_reemplazar_nodo(abb_nodo_t *actual);
static void borrar_sin_hijos(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, hijos hijo, bool es_raiz);
static void borrar_con_un_hijo(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, hijos relacion_act_ant, hijos hijo_de_act, bool es_raiz);
static bool borrar_con_dos_hijos(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual);
static void analizar_paternidad(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, const char *clave, void **dato, hijos relacion_act_ant);
static bool _abb_borrar(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, const char *clave, void **dato, hijos relacion_act_ant);
static void destruir_nodo(abb_nodo_t *actual, abb_destruir_dato_t destruir_dato);
static void _abb_destruir(abb_nodo_t *actual, abb_destruir_dato_t destruir_dato);
static abb_nodo_t *buscar_nodo(abb_nodo_t *actual, abb_nodo_t *anterior, const char *clave, abb_comparar_clave_t cmp);

static abb_nodo_t *abb_nodo_crear(const char *clave, void *dato)
{
    abb_nodo_t *abb_nodo = malloc(sizeof(abb_nodo_t));

    if (abb_nodo == NULL)
        return NULL;

    char *copia = malloc(strlen(clave) + 1);

    if (copia == NULL)
    {
        free(abb_nodo);
        return NULL;
    }

    strcpy(copia, clave);

    abb_nodo->clave = copia;
    abb_nodo->dato = dato;
    abb_nodo->izq = NULL;
    abb_nodo->der = NULL;

    return abb_nodo;
}

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato)
{
    abb_t *abb;

    if ((abb = malloc(sizeof(abb_t))) == NULL)
        return NULL;

    abb->raiz = NULL;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    abb->cantidad = 0;

    return abb;
}

static abb_nodo_t *buscar_nodo(abb_nodo_t *actual, abb_nodo_t *anterior, const char *clave, abb_comparar_clave_t cmp)
{
    if (actual == NULL)
    {
        return anterior;
    }

    int comparacion = cmp(clave, actual->clave);

    return comparacion == 0 ? actual : comparacion < 0 ? buscar_nodo(actual->izq, actual, clave, cmp)
                                                       : buscar_nodo(actual->der, actual, clave, cmp);
}

bool abb_guardar(abb_t *abb, const char *clave, void *dato)
{
    if (abb == NULL)
        return false;

    if (abb->cantidad == 0) // Caso arbol vacio
    {
        if ((abb->raiz = abb_nodo_crear(clave, dato)) == NULL)
            return false;
    }

    else
    {
        abb_nodo_t *auxiliar = buscar_nodo(abb->raiz, NULL, clave, abb->cmp);
        int comparacion = abb->cmp(clave, auxiliar->clave);

        if (comparacion == 0)
        {
            if (abb->destruir_dato != NULL)
            {
                abb->destruir_dato(auxiliar->dato);
            }
            auxiliar->dato = dato;
            return true;
        }

        else if (comparacion < 0)
        {
            if ((auxiliar->izq = abb_nodo_crear(clave, dato)) == NULL)
            {
                return false;
            }
        }

        else if (comparacion > 0)
        {
            if ((auxiliar->der = abb_nodo_crear(clave, dato)) == NULL)
            {
                return false;
            }
        }
    }

    // // La clave existe => es reemplazada
    // else if (recorrer(abb->raiz, clave, &dato, abb->cmp, abb->destruir_dato, REEMPLAZAR))
    //     return true;
    // // La clave no existe => se agrega
    // else if (!recorrer(abb->raiz, clave, &dato, abb->cmp, abb->destruir_dato, AGREGAR))
    //     return false;

    abb->cantidad++;

    return true;
}

static abb_nodo_t *buscar_y_reemplazar_nodo(abb_nodo_t *actual)
{
    abb_nodo_t *ant = actual;
    actual = actual->der;

    if (actual->izq == NULL)
    {
        ant->der = actual->der != NULL ? actual->der : NULL;
        return actual;
    }

    while (actual->izq != NULL)
    {
        ant = actual;
        actual = actual->izq;
    }

    ant->izq = actual->der != NULL ? actual->der : NULL;

    return actual;
}

static void borrar_sin_hijos(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, hijos hijo, bool es_raiz)
{
    if (es_raiz)
    {
        abb->raiz = NULL;
        return;
    }
    if (hijo == HIJO_DER)
        anterior->der = NULL;

    else
        anterior->izq = NULL;
}

static void borrar_con_un_hijo(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, hijos relacion_act_ant, hijos hijo_de_act, bool es_raiz)
{
    if (es_raiz)
    {
        abb->raiz = hijo_de_act == HIJO_DER ? actual->der : actual->izq;
        return;
    }

    if (relacion_act_ant == HIJO_IZQ)
        anterior->izq = hijo_de_act == HIJO_DER ? actual->der : actual->izq;
    else
        anterior->der = hijo_de_act == HIJO_DER ? actual->der : actual->izq;
}

static bool borrar_con_dos_hijos(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual)
{
    abb_nodo_t *reemplazante = buscar_y_reemplazar_nodo(actual);

    free(actual->clave);
    char *copia = malloc(strlen(reemplazante->clave) + 1);
    if (copia == NULL)
        return false;
    strcpy(copia, reemplazante->clave);

    actual->clave = copia;
    actual->dato = reemplazante->dato;

    destruir_nodo(reemplazante, NULL);

    return true;
}

static void analizar_paternidad(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, const char *clave, void **dato, hijos relacion_act_ant)
{
    bool es_raiz = false;
    *dato = actual->dato;

    if (anterior == NULL)
    {
        es_raiz = true;
    }

    if (actual->izq == NULL && actual->der == NULL)
    {
        borrar_sin_hijos(abb, anterior, actual, relacion_act_ant, es_raiz);
        destruir_nodo(actual, NULL);
    }

    else if (actual->izq == NULL ||  actual->der == NULL)
    {
        borrar_con_un_hijo(abb, anterior, actual, relacion_act_ant, actual->izq != NULL ? HIJO_IZQ : HIJO_DER, es_raiz);
        destruir_nodo(actual, NULL);
    }

    else
        borrar_con_dos_hijos(abb, anterior, actual);
}

static bool _abb_borrar(abb_t *abb, abb_nodo_t *anterior, abb_nodo_t *actual, const char *clave, void **dato, hijos relacion_act_ant)
{
    if (actual == NULL)
    {
        return false;
    }

    int cmp = abb->cmp(clave, actual->clave);

    if (cmp == 0)
    {
        analizar_paternidad(abb, anterior, actual, clave, dato, relacion_act_ant);
    }

    else if (cmp > 0)
    {
        return _abb_borrar(abb, actual, actual->der, clave, dato, HIJO_DER);
    }

    else
    {
        return _abb_borrar(abb, actual, actual->izq, clave, dato, HIJO_IZQ);
    }

    return true;
}

void *abb_borrar(abb_t *abb, const char *clave)
{
    if (abb == NULL || abb->cantidad == 0)
    {
        return NULL;
    }

    void *dato = NULL;

    if (!_abb_borrar(abb, NULL, abb->raiz, clave, &dato, SIN_RELACION))
    {
        return NULL;
    }

    abb->cantidad--;

    return dato;
}

void *abb_obtener(const abb_t *abb, const char *clave)
{
    if (abb == NULL)
        return NULL;

    // void *dato = NULL;

    abb_nodo_t *nodo_obtenido;

    if ((nodo_obtenido = buscar_nodo(abb->raiz, NULL, clave, abb->cmp)) == NULL)
    {
        return NULL;
    }

    return abb->cmp(nodo_obtenido->clave, clave) == 0 ? nodo_obtenido->dato : NULL;

    // return recorrer(abb->raiz, clave, &dato, abb->cmp, abb->destruir_dato, OBTENER) ? dato : NULL;
}

bool abb_pertenece(const abb_t *abb, const char *clave)
{
    if (abb == NULL)
        return false;

    abb_nodo_t *nodo_obtenido;

    if ((nodo_obtenido = buscar_nodo(abb->raiz, NULL, clave, abb->cmp)) == NULL)
    {
        return false;
    }

    return abb->cmp(nodo_obtenido->clave, clave) == 0;

    // return recorrer(abb->raiz, clave, NULL, abb->cmp, abb->destruir_dato, ENCONTRAR);
}

size_t abb_cantidad(abb_t *abb)
{
    return abb == NULL ? 0 : abb->cantidad;
}

static void destruir_nodo(abb_nodo_t *actual, abb_destruir_dato_t destruir_dato)
{
    if (destruir_dato != NULL)
        destruir_dato(actual->dato);
    free(actual->clave);
    free(actual);
}

static void _abb_destruir(abb_nodo_t *actual, abb_destruir_dato_t destruir_dato)
{
    // Si el abb esta vacio
    if (actual == NULL)
        return;

    if (actual->izq != NULL)
        _abb_destruir(actual->izq, destruir_dato);

    if (actual->der != NULL)
        _abb_destruir(actual->der, destruir_dato);

    destruir_nodo(actual, destruir_dato);
}

void abb_destruir(abb_t *abb)
{
    _abb_destruir(abb->raiz, abb->destruir_dato);
    free(abb);
}

static void _abb_in_order(abb_nodo_t *actual, bool visitar(const char *, void *, void *), void *extra, bool *continuar)
{
    if (actual == NULL || visitar == NULL || !(*continuar))
        return;

    // Izq - Actual - Der
    _abb_in_order(actual->izq, visitar, extra, continuar);
    if (*continuar)
    {
        *continuar &= visitar(actual->clave, actual->dato, extra);
    }
    _abb_in_order(actual->der, visitar, extra, continuar);
}

void abb_in_order(abb_t *abb, bool visitar(const char *, void *, void *), void *extra)
{
    if (abb == NULL)
        return;
    bool continuar = true;
    _abb_in_order(abb->raiz, visitar, extra, &continuar);
}

void establecer_orden_iteracion(abb_nodo_t *actual, pila_t *estados)
{
    if (actual == NULL)
        return;
    pila_apilar(estados, actual);
    establecer_orden_iteracion(actual->izq, estados); //apilar estados iniciales
}

abb_iter_t *abb_iter_in_crear(const abb_t *abb)
{
    if (abb == NULL)
        return NULL;

    abb_iter_t *iter;

    if ((iter = malloc(sizeof(abb_iter_t))) == NULL)
        return NULL;

    if ((iter->estados = pila_crear()) == NULL)
    {
        free(iter);
        return NULL;
    }

    establecer_orden_iteracion(abb->raiz, iter->estados);

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter)
{
    if (abb_iter_in_al_final(iter))
        return false;

    abb_nodo_t *actual;

    if ((actual = pila_desapilar(iter->estados)) == NULL)
        return false;

    establecer_orden_iteracion(actual->der, iter->estados);

    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
    return pila_esta_vacia(iter->estados) ? NULL : ((abb_nodo_t *)pila_ver_tope(iter->estados))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter)
{
    return pila_esta_vacia(iter->estados);
}

void abb_iter_in_destruir(abb_iter_t *iter)
{
    pila_destruir(iter->estados);
    free(iter);
}