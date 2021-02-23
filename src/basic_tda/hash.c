#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include "hash.h"

#define CAPACIDAD_INICIAL 128
#define AUMENTO 2   // Por cuanto multiplica la capacidad
#define REDUCCION 2 // Por cuanto divide la capacidad
#define FACTOR_REDIMENSION_AUMENTO 70
#define FACTOR_REDIMENSION_REDUCCION 25

#define OBTENER_FACTOR_CARGA(cantidad, capacidad) (cantidad * 100) / capacidad
#define OBTENER_CANTIDAD_TOTAL(hash) hash->cantidad_borrados + hash->cantidad_ocupados

// Definicion de estados.

typedef enum estados {
    VACIO = 0,
    OCUPADO = 1,
    BORRADO = 2
} estados;

// Prototipos de funciones auxiliares.

size_t determinar_redimension(hash_t *hash);
bool redimensionar(hash_t* hash, size_t nueva_capacidad);
bool buscar_coincidencia(const hash_t *hash, const char *clave, size_t *posicion);
void destruir_campo(hash_t *hash, size_t posicion);
void destruir_tabla(hash_t *hash);

// Definiciones de structs.

typedef struct hash_campo {
    void *dato;
    char *clave;
    estados estado;
} hash_campo_t;

typedef struct hash {
    hash_campo_t *tabla;
    size_t cantidad_ocupados;
    size_t cantidad_borrados;
    size_t capacidad;
    hash_destruir_dato_t destruir_dato;
} hash_t;

typedef struct hash_iter{
    const hash_t *hash;
    size_t posicion;
} hash_iter_t;

// Implementacion de primitivas y funciones auxiliares.

// Jenkins hash function
// one_at_a_time
// https://en.wikipedia.org/wiki/Jenkins_hash_function
size_t f_hash(const char* clave) {
  size_t i = 0;
  size_t hash = 0;

  while (clave[i] != '\0') {
    hash += (uint8_t)clave[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }

  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash;
}

size_t determinar_redimension(hash_t *hash) {
    size_t cantidad_total = OBTENER_CANTIDAD_TOTAL(hash);
    size_t factor_carga = OBTENER_FACTOR_CARGA(cantidad_total, hash->capacidad);

    if (factor_carga >= FACTOR_REDIMENSION_AUMENTO)
        return hash->capacidad * AUMENTO;

    if (hash->capacidad > CAPACIDAD_INICIAL && factor_carga <= FACTOR_REDIMENSION_REDUCCION)
        return hash->capacidad / REDUCCION;
    
    // NO hay que redimensionar.
    return 0;
}

bool redimensionar(hash_t* hash, size_t nueva_capacidad) {
    hash_campo_t *nueva_tabla = calloc(nueva_capacidad, sizeof(hash_campo_t));

    if (nueva_tabla == NULL)
        return false;

    // Transfiere los datos de la tabla vieja a la nueva.
    for (size_t i = 0; i < hash->capacidad; i++) {
        if (hash->tabla[i].estado == OCUPADO) {
            size_t nueva_posicion = f_hash(hash->tabla[i].clave) % nueva_capacidad;
            while (nueva_tabla[nueva_posicion].estado == OCUPADO)
                if (++nueva_posicion == nueva_capacidad) 
                    nueva_posicion = 0;
            nueva_tabla[nueva_posicion].clave = hash->tabla[i].clave;
            nueva_tabla[nueva_posicion].dato = hash->tabla[i].dato;
            nueva_tabla[nueva_posicion].estado = OCUPADO;
        }
    }

    free(hash->tabla);
    hash->tabla = nueva_tabla;
    hash->capacidad = nueva_capacidad;
    hash->cantidad_borrados = 0;

    return true;
}


hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t *hash = malloc(sizeof(hash_t));

    if (hash == NULL)
        return NULL;
    
    hash->tabla = calloc(CAPACIDAD_INICIAL, sizeof(hash_campo_t));

    if (hash->tabla == NULL) {
        free(hash);
        return NULL;
    }

    hash->cantidad_ocupados = hash->cantidad_borrados = 0;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->destruir_dato = destruir_dato;

    return hash;
}

/* Busca una una coincidencia de claves (campo ocupado con una clave especifica)
y actualiza la posicion. */
bool buscar_coincidencia(const hash_t *hash, const char *clave, size_t *posicion) {
    size_t posicion_busqueda = f_hash(clave) % (hash->capacidad);

    while (hash->tabla[posicion_busqueda].estado != VACIO) {
        if ((hash->tabla[posicion_busqueda].estado == OCUPADO) &&
                strcmp(hash->tabla[posicion_busqueda].clave, clave) == 0) {
            if (posicion != NULL)    
                *posicion = posicion_busqueda;
            return true;
        }

        if (++posicion_busqueda == hash->capacidad)
            posicion_busqueda = 0;
    }

    return false;
}

void buscar_libre(hash_t *hash, size_t *posicion) {
    while (hash->tabla[*posicion].estado == OCUPADO) 
        if (++(*posicion) == hash->capacidad)
            *posicion = 0;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    size_t nueva_capacidad = determinar_redimension(hash);

    if (nueva_capacidad && !redimensionar(hash, nueva_capacidad))
        return false;

    size_t posicion = f_hash(clave) % hash->capacidad;
    
    if (buscar_coincidencia(hash, clave, &posicion)) {
        if (hash->destruir_dato != NULL)
            hash->destruir_dato(hash->tabla[posicion].dato);
    }

    else {
        buscar_libre(hash, &posicion);
        char *copia = malloc(strlen(clave) + 1);
        if (copia == NULL) 
            return false;
        strcpy(copia, clave);
        if (hash->tabla[posicion].estado == BORRADO)
            hash->cantidad_borrados--;
        hash->cantidad_ocupados++;
        hash->tabla[posicion].clave = copia;
    }
        
    hash->tabla[posicion].dato = dato;
    hash->tabla[posicion].estado = OCUPADO;

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    size_t nueva_capacidad = determinar_redimension(hash);

    if (nueva_capacidad && !redimensionar(hash, nueva_capacidad))
        return NULL;

    size_t posicion;
    if (!buscar_coincidencia(hash, clave, &posicion)) 
        return NULL;

    free(hash->tabla[posicion].clave);
    hash->tabla[posicion].estado = BORRADO;
    hash->cantidad_ocupados--;
    hash->cantidad_borrados++;

    return hash->tabla[posicion].dato;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    size_t posicion;
    if (!buscar_coincidencia(hash, clave, &posicion)) 
        return NULL; 

    return hash->tabla[posicion].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    return buscar_coincidencia(hash, clave, NULL);
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad_ocupados;
}

void destruir_campo(hash_t *hash, size_t posicion) {
    if (hash->destruir_dato != NULL)
        hash->destruir_dato(hash->tabla[posicion].dato);
    free(hash->tabla[posicion].clave);
}

void hash_destruir_tabla(hash_t *hash) {
    for (size_t i = 0; i < hash->capacidad; i++)
        if (hash->tabla[i].estado == OCUPADO)
            destruir_campo(hash, i);
    free(hash->tabla);
}

void hash_destruir(hash_t *hash) {
    hash_destruir_tabla(hash);
    free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));

    if (iter == NULL)
        return NULL;
    
    iter->hash = hash;
    iter->posicion = 0;

    if (iter->hash->tabla[iter->posicion].estado == VACIO)
        hash_iter_avanzar(iter);

    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter))
        return false;

    iter->posicion++; // Minimo avanza una vez.
    while (!hash_iter_al_final(iter) &&
            iter->hash->tabla[iter->posicion].estado != OCUPADO)
            iter->posicion++;

    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    return hash_iter_al_final(iter) ? NULL : iter->hash->tabla[iter->posicion].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->posicion == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}