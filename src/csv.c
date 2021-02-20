#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#define SEPARATION ','

static void remove_new_line(char* line) {
	size_t len = strlen(line);
	if (line[len - 1] == '\n') {
		line[len - 1] = '\0';
	}
}

lista_t* csv_create_strucure(const char* csv_path, void* (*creador) (char**, void*), void* extra) {
	FILE* file = fopen(csv_path, "r");
	if (!file) {
		return NULL;
	}
	
	lista_t* list = lista_crear();
	if (!list) {
		fclose(file);
		return NULL;
	}

	char* line = NULL;
	size_t c = 0;
	while (getline(&line, &c, file) > 0) {
		remove_new_line(line);
		char** values = split(line, SEPARATION);
		lista_insertar_ultimo(list, creador(values, extra));
		free_strv(values);
	}
	free(line);
	fclose(file);
	return list;
}

