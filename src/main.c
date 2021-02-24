#define  _POSIX_C_SOURCE 200809L // para getline

#include <stdio.h>	/* perror */
#include <errno.h>	/* errno */
#include <stdlib.h> /* malloc, free, exit */
#include <stdbool.h>

/* Inclución de estructuras */
#include "basic_tda/lista.h"
#include "our_tda/doctor/bst_doctors.h"
#include "our_tda/patient/hash_patients.h"
#include "our_tda/turns/hash_turns.h"
#include "csv.h"
/* Inclución de librerías de funciones propias */
#include "function_libraries/strutil.h"
#include "function_libraries/load_structure_functions.h"
#include "function_libraries/command_functions.h"
/* Inclución de librerías de mensajes */
#include "message_libraries/error_messages.h"

#define CMD_PEDIR_TURNO "PEDIR_TURNO"
#define CMD_ATENDER "ATENDER_SIGUIENTE"
#define CMD_INFORME "INFORME"

/* Firma de funciones auxiliares */

/* Funciones relacionadas con manejo de errores */
static int handle_command_error(int argc);
static int handle_file_error(int argc, char *path);
static bool handle_format_error(char **values, char *line);
/* Funciones relacionadas con la generación de estrucutras */
static lista_t *process_doctors_data(FILE *doctors_dile);
static lista_t *process_patients_data(FILE *patients_file, lista_t *doctors_data);
static BSTDoctors *register_doctors(lista_t *doctors_data, lista_t *patients_data);
static HashPatients *register_patients(BSTDoctors *doctors_register, lista_t *patients_data);
static HashTurns *init_hash_turns(lista_t *doctors_data, lista_t *patients_data);
/* Funciones relacionadas con la fase de comandos */
static bool process_command(char *cmd, char **parameters,
							HashTurns *turns, HashPatients *patients, BSTDoctors *doctors);
static void remove_new_line(char *line);
static void process_stdin(HashTurns *turns, HashPatients *patients, BSTDoctors *doctors);

int main(int argc, char **argv)
{
	handle_command_error(argc);

	char *doctors_path = argv[1];
	char *patients_path = argv[2];

	FILE *doctors_file, *patients_file;

	doctors_file = fopen(doctors_path, "r");
	handle_file_error(errno, doctors_path);

	patients_file = fopen(patients_path, "r");
	handle_file_error(errno, patients_path);

	lista_t *doctors_data = process_doctors_data(doctors_file);
	lista_t *patients_data = process_patients_data(patients_file, doctors_data);
	fclose(doctors_file);
	fclose(patients_file);
	BSTDoctors *doctors_register = register_doctors(doctors_data, patients_data);
	HashTurns *turns = init_hash_turns(doctors_data, patients_data);
	destroy_structure(doctors_data);
	HashPatients *patients_register = register_patients(doctors_register, patients_data);
	destroy_structure(patients_data);

	process_stdin(turns, patients_register, doctors_register);

	hash_turns_destroy(turns);
	bst_doctors_destroy(doctors_register);
	hash_patients_destroy(patients_register);

	return 0;
}

static bool process_command(char *cmd, char **parameters,
							HashTurns *turns, HashPatients *patients, BSTDoctors *doctors)
{

	if (strcmp(cmd, CMD_PEDIR_TURNO) == 0)
	{
		make_appointment(turns, patients, parameters);
	}

	else if (strcmp(cmd, CMD_ATENDER) == 0)
	{
		attend_patient(turns, doctors, parameters);
	}

	else if (strcmp(cmd, CMD_INFORME) == 0)
	{
		generate_report(doctors, parameters);
	}

	else
	{
		printf(ERROR_CMD, cmd);
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

static bool _handle_format_error(char **values)
{
	size_t values_count;
	for (values_count = 1; values[values_count-1] != NULL; values_count++);
	if (values_count < 3)
	{
		return false;
	}
	return true;
}

static bool handle_format_error(char **values, char *line)
{
	if (values == NULL || !_handle_format_error(values))
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
		if (!handle_format_error(values, line))
			continue;
		char **parameters = split(values[1], ',');
		process_command(values[0], parameters, turns, patients, doctors);
		free_strv(parameters);
		free_strv(values);
	}
	free(line);
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

static lista_t *process_patients_data(FILE *patients_file, lista_t *doctors_data)
{
	lista_t *patients_data;
	if ((patients_data = csv_create_structure(patients_file)) == NULL)
	{
		destroy_structure(doctors_data);
		printf(ERROR_MEM, "patients_data");
		exit(EXIT_FAILURE);
	}
	return patients_data;
}

static lista_t *process_doctors_data(FILE *doctors_file)
{
	lista_t *doctors_data;
	if ((doctors_data = csv_create_structure(doctors_file)) == NULL)
	{
		printf(ERROR_MEM, "doctors_data");
		exit(EXIT_FAILURE);
	}
	return doctors_data;
}

static HashTurns * init_hash_turns(lista_t *doctors_data, lista_t *patients_data)
{
	HashTurns *turns;
	if ((turns = load_hash_turns(doctors_data)) == NULL)
	{
		destroy_structure(patients_data);
		destroy_structure(doctors_data);
		printf(ERROR_MEM, "turns");
		exit(EXIT_FAILURE);
	}
	return turns;
}

static BSTDoctors *register_doctors(lista_t *doctors_data, lista_t *patients_data)
{
	BSTDoctors *doctors_register;
	if ((doctors_register = load_doctors(doctors_data)) == NULL)
	{
		destroy_structure(doctors_data);
		destroy_structure(patients_data);
		printf(ERROR_MEM, "doctors_register");
		exit(EXIT_FAILURE);
	}
	return doctors_register;
}

static HashPatients *register_patients(BSTDoctors *doctors_register, lista_t *patients_data)
{
	HashPatients *patients_register;
	if ((patients_register = load_patients(patients_data)) == NULL)
	{
		bst_doctors_destroy(doctors_register);
		destroy_structure(patients_data);
		printf(ERROR_MEM, "patients_register");
		exit(EXIT_FAILURE);
	}
	return patients_register;
}