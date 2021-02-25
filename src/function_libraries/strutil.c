#define _POSIX_C_SOURCE 200809L  // strdup(), stpcpy()

// Librerias generales.

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Librerias locales.

#include "strutil.h"

char *substr(const char *str, size_t n) {
    // Esta implementacion de strlen la hago para mantener la complejidad O(k),
    // Ya que si k es la longitud del sub_string entonces en el caso de que
    // k sea mayor a la longitud del str de entrada entonces en el malloc
    // reservaria mas memoria de la necesitada. Por eso este for asegura
    // que siempre se reservara la memoria justa y necesaria sin complejizar
    // aun mas el algoritmo.
    size_t len;
    for (len = 0; str[len] != '\0' && len < n; len++);

    // Reserva memoria para el sub_string
    char *sub_string = malloc(len + 1);

    // Si no se pudo reservar suficiente memoria, devuelvo NULL
    if (sub_string == NULL)
        return NULL;
    
    // Copio len caracteres de str a sub_string
    strncpy(sub_string, str, len);
    // Agrego el caracter nulo al final
    sub_string[len] = '\0';

    return sub_string;
}

char **split(const char *str, char sep) {
    size_t sep_count = 0;
    for (size_t i = 0; str[i] != '\0'; i++)
        if (str[i] == sep)
            sep_count++;

    // Reserva memeoria para el arreglo de cadenas dinamicas
    char **strv = malloc(sizeof(char *) * (sep_count + 2)); // Sumo 2 por el str final y el NULL

    // Si no se pudo reservar suficiente memoria, devuelvo NULL
    if (strv == NULL)
        return NULL;

    // SAVE_SPLIT es una macro que guarda la separacion de la cadena inicial en
    // el arreglo dinamico de cadenas dinamicas.
    // En caso de NULL, libera el string dinamico.
    // Esta definida solo en este tramo de la funcion para que no interfiera con
    // el resto del codigo.
    #define SAVE_SPLIT(start, length) \
        if ((strv[i++] = substr(str + start, length)) == NULL) {\
            free_strv(strv);\
            return NULL;\
        }

    size_t i = 0, n, n_corte;
    for (n = 0, n_corte = 0; str[n] != '\0'; n++) {
        // Recorre el arreglo hasta encontrar la separacion, y guarda en el
        // arreglo dinamico el sub_string de la cadena de entrada con la
        // cantidad de caracteres totales menos la cantidad de caracteres
        // recorridos hasta el corte.
        if (str[n] == sep) {
            SAVE_SPLIT(n_corte, n - n_corte);
            n_corte = n + 1;
        }
    }
    // Guarda el string final.
    SAVE_SPLIT(n_corte, n - n_corte);

    #undef SAVE_SPLIT

    // El ultimo elemento del arreglo dinamico es NULL.
    strv[i] = NULL;

    return strv;
}

char *join(char **strv, char sep) {

    size_t i, total_chars = 0;

    // Cuenta la cantidad de caracteres totales de todas las cadenas del
    // arreglo dinamico
    for (i = 0; strv[i] != NULL; i++)
        total_chars += strlen(strv[i]) + 1;

    size_t str_count = i;
    
    // Si el separador no es '\0' y strv no es NULL
    if (sep != '\0' && str_count > 0)
        total_chars += str_count - 1;

    // Si strv no es solo NULL
    if (str_count > 0)
        total_chars--;
    
    // Reserva memoria para la cadena final
    char *joined = malloc(total_chars + 1);

    // Si no se pudo reservar suficiente memoria, devuelvo NULL
    if (joined == NULL)
        return NULL;

    size_t j = 0;
    for (i = 0; i < str_count; i++) {
        size_t length = strlen(strv[i]);
        // Copia todos los caracteres de cada cadena del arreglo dinamico
        // sin incluir el caracter nulo final en ningun caso
        strcpy(joined + j, strv[i]);
        j += length;
        // Inserta la separacion en la union entre las cadenas del arreglo dinamico
        if (sep != '\0')
            joined[j++] = sep;
    }

    // Inserta el caracter nulo al final de la cadena final
    if (sep != '\0' && i > 0)
        joined[j - 1] = '\0';
    else
        joined[j] = '\0';

    // Devuelve la cadena final unida por la separacion
    return joined;
}

void free_strv(char *strv[]) {
    // Libera cada elemento del arreglo dinamico.
    for (size_t i = 0; strv[i] != NULL; free(strv[i++]));
    // Libera el arreglo dinamico.
    free(strv);
}