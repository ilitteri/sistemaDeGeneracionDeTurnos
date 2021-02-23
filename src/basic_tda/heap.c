#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "heap.h"

#define TAM_INICIAL 8
#define FACTOR_TAM 2
#define FACTOR_AUMENTO 2      // Por cuanto multiplico la capacidad
#define FACTOR_REDUCCION 2    // Por cuanto divido la capacidad
#define LIMITE_DE_REDUCCION 4 // Cuantas veces debe ser mayor la capacidad a la cantidad para reducirla

/* Definicion de estructuras */

typedef struct heap
{
    void **datos;
    size_t tam;
    size_t cant;
    cmp_func_t cmp;
} heap_t;

/* Declaracion de funciones auxiliares */

void swap(void **datos, size_t i, size_t j);
size_t obtener_pos_padre(size_t i);
size_t obtener_pos_hijo_izq(size_t i);
size_t obtener_pos_hijo_der(size_t i);
void upheap(void **datos, size_t i, cmp_func_t cmp);
void downheap(void **datos, size_t cant, size_t actual, cmp_func_t cmp);
bool redimensionar(heap_t *heap, size_t nuevo_tam);
void heapify(void *elementos[], size_t n, cmp_func_t cmp);
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

void swap(void **datos, size_t i, size_t j)
{
    void *temp = datos[i];
    datos[i] = datos[j];
    datos[j] = temp;
}

size_t obtener_pos_padre(size_t i)
{
    return (i - 1) / 2;
}

size_t obtener_pos_hijo_izq(size_t i)
{
    return i * 2 + 1;
}

size_t obtener_pos_hijo_der(size_t i)
{
    return i * 2 + 2;
}

void upheap(void **datos, size_t i, cmp_func_t cmp)
{
    if (i == 0)
    {
        return;
    }

    size_t j = obtener_pos_padre(i);
    if (cmp(datos[i], datos[j]) > 0)
    {
        swap(datos, i, j);
        upheap(datos, j, cmp);
    }
}

void downheap(void **datos, size_t cant, size_t actual, cmp_func_t cmp)
{
    if (actual >= cant)
    {
        return;
    }

    size_t hijo_izq = obtener_pos_hijo_izq(actual);
    size_t hijo_der = obtener_pos_hijo_der(actual);
    size_t mayor = actual;

    if (hijo_der < cant && cmp(datos[mayor], datos[hijo_der]) < 0)
    {
        mayor = hijo_der;
    }

    if (hijo_izq < cant && cmp(datos[mayor], datos[hijo_izq]) < 0)
    {
        mayor = hijo_izq;
    }

    if (mayor != actual)
    {
        swap(datos, actual, mayor);
        downheap(datos, cant, mayor, cmp);
    }
}

bool redimensionar(heap_t *heap, size_t nuevo_tam)
{
    void **nuevos_datos = realloc(heap->datos, sizeof(void *) * nuevo_tam);

    if (nuevos_datos == NULL)
    {
        return false;
    }

    heap->tam = nuevo_tam;
    heap->datos = nuevos_datos;

    return true;
}

void heapify(void *elementos[], size_t n, cmp_func_t cmp)
{
    size_t i = n / 2;
    while (i--)
    {
        downheap(elementos, n, i, cmp);
    }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp)
{
    if (elementos == NULL || !cant)
    {
        return;
    }

    heapify(elementos, cant, cmp);

    size_t n = cant;
    while (n--)
    {
        swap(elementos, 0, n);
        downheap(elementos, n, 0, cmp);
    }
}

heap_t *_heap_crear(size_t tam, size_t cant, cmp_func_t cmp)
{
    heap_t *heap;

    if ((heap = malloc(sizeof(heap_t))) == NULL)
    {
        return NULL;
    }

    if ((heap->datos = malloc(sizeof(void *) * tam)) == NULL)
    {
        free(heap);
        return NULL;
    }

    heap->tam = tam;
    heap->cant = cant;
    heap->cmp = cmp;

    return heap;
}

heap_t *heap_crear(cmp_func_t cmp)
{
    heap_t *heap;

    if ((heap = _heap_crear(TAM_INICIAL, 0, cmp)) == NULL)
    {
        return NULL;
    }

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp)
{
    heap_t *heap;

    if ((heap = _heap_crear(FACTOR_TAM * n, n, cmp)) == NULL)
    {
        return NULL;
    }

    memcpy(heap->datos, arreglo, n * sizeof(void *)); // no tocamos el arreglo.
    heapify(heap->datos, n, cmp);                     // asegura propiedad de heap en el arreglo.

    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e))
{
    for (size_t i = 0; i < heap->cant; i++)
    {
        if (destruir_elemento != NULL)
        {
            destruir_elemento(heap->datos[i]);
        }
    }

    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap)
{
    return heap != NULL ? heap->cant : 0;
}

bool heap_esta_vacio(const heap_t *heap)
{
    return !heap->cant;
}

bool heap_encolar(heap_t *heap, void *elem)
{
    if (heap->cant == heap->tam)
    {
        if (!redimensionar(heap, heap->cant * FACTOR_AUMENTO))
        {
            return false;
        }
    }
    heap->datos[heap->cant] = elem;
    upheap(heap->datos, heap->cant, heap->cmp);
    heap->cant++;
    return true;
}
void *heap_ver_max(const heap_t *heap)
{
    return heap_esta_vacio(heap) ? NULL : heap->datos[0];
}

void *heap_desencolar(heap_t *heap)
{
    if (heap_esta_vacio(heap))
    {
        return NULL;
    }

    void *dato = heap->datos[0];

    heap->datos[0] = heap->datos[heap->cant - 1];

    downheap(heap->datos, heap->cant, 0, heap->cmp);

    heap->cant--;

    if (heap->cant < heap->tam / LIMITE_DE_REDUCCION && heap->tam > TAM_INICIAL)
    {
        redimensionar(heap, heap->tam / FACTOR_REDUCCION);
    }

    return dato;
}