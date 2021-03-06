#define _POSIX_C_SOURCE 200809L //getline

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

#define SEPARATION ','

static void remove_new_line(char *line);
static void _free_strv(void *strv);

static void remove_new_line(char *line)
{
	size_t len = strlen(line);
	if (line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
	}
}

static void _free_strv(void *strv)
{
	free_strv((char **)strv);
}

lista_t *csv_create(FILE *file)
{
	lista_t *list = lista_crear();
	if (!list)
	{
		fclose(file);
		return NULL;
	}

	char *line = NULL;
	size_t c = 0;
	while (getline(&line, &c, file) > 0)
	{
		remove_new_line(line);
		char **values = split(line, SEPARATION);
		lista_insertar_ultimo(list, values);
	}
	free(line);
	return list;
}

void csv_destroy(lista_t *list)
{
	lista_destruir(list, _free_strv);
}