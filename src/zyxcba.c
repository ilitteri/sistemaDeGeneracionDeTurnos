#include <stdio.h>	/* perror */
#include <errno.h>	/* errno */
#include <stdlib.h> /* malloc, free, exit */
#include <stdbool.h>

/* Inclución de estructuras */
#include "lista.h"
#include "bst_doctors.h"
#include "hash_patients.h"
#include "hash_turns.h"
/* Inclución de librerías de funciones propias */
#include "strutil.h"
#include "load_structure_functions.h"
/* Inclución de librerías de mensajes */
#include "error_messages.h"

#define CMD_SIZE 128

#define CMD_PEDIR_TURNO "PEDIR_TURNO"
#define CMD_ATENDER "ATENDER_SIGUIENTE"
#define CMD_INFORME "INFORME"

/* Firma de funciones auxiliares */

/* Funciones relacionadas con manejo de errores */
static int handle_command_error(int argc);
static int handle_file_error(int argc, char *path);
static bool handle_format_error(char *parameters);
/* Funciones relacionadas con la generación de estrucutras */
static void process_patients_data(FILE *patients_file, FILE *doctors_file, lista_t *patients_data, lista_t *doctors_data);
static void process_doctors_data(FILE *patients_file, FILE *doctors_file, lista_t *doctors_data);
static void register_doctors(FILE *patients_file, FILE *doctors_file, BSTDoctors *doctors_register, lista_t *doctors_data, lista_t *patients_data);
static void register_patients(FILE *patients_file, FILE *doctors_file, BSTDoctors *doctors_register, HashPatients *patients_register, lista_t *patients_data);
static void load_structures(FILE *doctors_file, FILE *patients_file,
							BSTDoctors *doctors_register, HashPatients *patients_register,
							lista_t *doctors_data, lista_t *patients_data);
/* Funciones relacionadas con archivos */
static void close_files(FILE *doctors, FILE *patients);
/* Funciones relacionadas con la fase de comandos */
static bool process_command(char *cmd, char **parameters,
							HashTurns *turns, HashPatients *patients, BSTDoctors *doctors);
static void remove_new_line(char* line);
static void process_stdin(HashTurns *turns, HashPatients *patients, BSTDoctors *doctors);

int main(int argc, char **argv) 
{
    handle_command_error(argc);

    char *doctors_path = argv[1];
    char *patients_path = argv[2];

    FILE *doctors_file, *patients_file;

    doctors_file = fopen(doctors_path, "r"); 
    handle_file_error(errno, doctors_path);

    patients_file = fopen(doctors_path, "r"); 
    handle_file_error(errno, patients_path);

    lista_t *doctors_data;
    lista_t *patients_data;
    BSTDoctors *doctors_register;
    HashPatients *patients_register;
    HashTurns *turns;

    load_structures(doctors_file, patients_file, doctors_register, patients_register, doctors_data, patients_data, turns);
    close_files(doctors_file, patients_file);

    process_stdin();

    return 0; 
} 

static bool process_command(char *cmd, char **parameters,
							HashTurns *turns, HashPatients *patients, BSTDoctors *doctors)
{

	if (strcmp(cmd, CMD_PEDIR_TURNO) == 0)
	{
		pedir_turno(turns, patients, parameters);
	}

	else if (strcmp(cmd, CMD_ATENDER) == 0)
	{
		atender_paciente(turns, doctors, parameters);
	}

	else if (strcmp(cmd, CMD_INFORME) == 0)
	{
		generar_informe(doctors, parameters);
	}

	else
	{
		return false;
	}

	return true;
}

void remove_new_line(char *line)
{
	size_t len = strlen(line);
	if (line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
	}
}

static bool handle_format_error(char *parameters)
{
	if (values[1] == NULL)
	{
		printf(ERROR_FORMAT, line);
		free_strv(values);
		return false;
	}
	return true;
}

void process_stdin(HashTurns *turns, HashPatients *patients, BSTDoctors *doctors)
{
	char *line = NULL;
	size_t c = 0;
	while (getline(&line, &c, stdin) > 0)
	{
		remove_new_line(line);
		char **values = split(line, ':');
		if (!handle_format_error(values[1]))
			continue;
		char **parameters = split(values[1], ',');
		process_command(values[0], parameters, turns, patients, doctors);
		free_strv(parameters);
		free_strv(values);
	}
	free(line);
}

static void close_files(FILE *doctors_file, FILE *patients_file)
{
	fclose(doctors_file);
	fclose(patients_file);
}

static int handle_command_error(int argc)
{
	if (argc != 3)
	{
		printf(ERROR_PARAMS_COUNT);
		exit(EXIT_FAILURE);
	}
	return 0;
}

static int handle_file_error(int errnum, char *path)
{
	if (errnum == 2)
	{
		printf(ERROR_FILE, path);
		exit(EXIT_FAILURE);
	}
	return 0;
}

static void process_patients_data(FILE *patients_file, FILE *doctors_file, lista_t *patients_data, lista_t *doctors_data)
{
	if ((patients_data = csv_create_structure(patients_file)) == NULL)
	{
		destroy_structure(doctors_data);
		close_files(doctors_file, patients_file);
		printf(ERROR_MEM, "patients_data");
		exit(EXIT_FAILURE);
	}
}

static void process_doctors_data(FILE *patients_file, FILE *doctors_file, lista_t *doctors_data)
{
	if ((doctors_data = csv_create_structure(doctors_file)) == NULL)
	{
		close_files(doctors_file, patients_file);
		printf(ERROR_MEM, "doctors_data");
		exit(EXIT_FAILURE);
	}
}

static void init_hash_turns(FILE *patiens_file, FILE *doctors_file,
							lista_t *doctors_data, lista_t *patients_data,
							HashTurns *turns)
{
	if ((turns = load_hash_turns(doctors_data)) == NULL)
	{
		destroy_structure(patients_data);
		destroy_structure(doctors_data);
		close_files(patiens_file, doctors_file);
		printf(ERROR_MEM, "turns");
		exit(EXIT_FAILURE);
	}
}

static void register_doctors(FILE *patients_file, FILE *doctors_file, BSTDoctors *doctors_register, lista_t *doctors_data, lista_t *patients_data)
{
	if ((doctors_register = load_doctors(doctors_data)) == NULL)
	{
		destroy_structure(doctors_data);
		destroy_structure(patients_data);
		close_files(doctors_file, patients_file);
		printf(ERROR_MEM, "doctors_register");
		exit(EXIT_FAILURE);
	}
}

static void register_patients(FILE *patients_file, FILE *doctors_file, BSTDoctors *doctors_register, HashPatients *patients_register, lista_t *patients_data)
{
	if ((patients_register = load_patients(patients_data)) == NULL)
	{
		bst_doctors_destroy(doctors_register);
		destroy_structure(patients_data);
		close_files(doctors_file, patients_file);
		printf(ERROR_MEM, "patients_register");
		exit(EXIT_FAILURE);
	}
}

static void load_structures(FILE *doctors_file, FILE *patients_file,
							BSTDoctors *doctors_register, HashPatients *patients_register,
							lista_t *doctors_data, lista_t *patients_data)
{
	process_doctors_data(patients_file, doctors_file, doctors_data);
	process_patients_data(patients_file, doctors_file, patients_data, doctors_data);
	init_hash_turns(patiens_file, doctors_file, doctors_data, patients_data, turns);
	register_doctors(patients_file, doctors_file, doctors_register, doctors_data, patients_data);
	destroy_structure(doctors_data);
	register_patients(patients_file, doctors_file, doctors_register, patients_register, patients_data);
}