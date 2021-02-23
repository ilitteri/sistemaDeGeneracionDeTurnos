#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* Inclución de librerías de mensajes */
#include "../message_libraries/error_messages.h"
#include "../message_libraries/success_messages.h"
/* Inclución de estructuras */
#include "../our_tda/doctor/doctor.h"
#include "../our_tda/patient/patient.h"
#include "../our_tda/command/report.h"
#include "../our_tda/doctor/bst_doctors.h"
#include "../our_tda/patient/hash_patients.h"
#include "../our_tda/turns/hash_turns.h"

/* Firma de funciones auxiliares */
static bool cmd1_error_handler(char** parameters, HashTurns *turns, HashPatients *patients);
static void cmd1_print_result(char *patient_name, char *specialty, size_t n);
static bool ask_turn(char** parameters, HashTurns *turns, HashPatients *patients);
/* Funciones auxiliares para el comando 2 */
static bool cmd2_error_handler(char** parameters, HashTurns *turns, BSTDoctors *doctors);
static void cmd2_print_result(char *patient_name, char *specialty, size_t n);
static bool _attend_patient(char** parameters, HashTurns *turns, Doctor *doctors, Patient **patient);
/* Funciones auxiliares para el comando 3 */
static bool cm3_error_handler(char **parameters);
static bool verify_doctor_existance(BSTDoctors *doctors, char **parameters);
static bool visit_doctors_in_range(const char *clave, void *dato, void *extra);
static void execute_cmd3(BSTDoctors *doctors, const char *min, const char *max);

void make_appointment(char** parameters, HashTurns *turns, HashPatients *patients)
{
    if (!cmd1_error_handler(parameters, turns, patients))
    {
        return;
    }

    if (!ask_turn(parameters, turns, patients))
    {
        return;
    }

    cmd1_print_result(parameters[0], parameters[1], hash_turns_specialty_count(turns, parameters[1])-1);
}

void attend_patient(char** parameters, HashTurns *turns, BSTDoctors *doctors)
{
    if (!cmd2_error_handler(parameters, turns, doctors))
    {
        return;
    }

    Doctor *doctor = bst_doctors_get_doctor(doctors, parameters[0]);
    Patient *patient;
    if (!_attend_patient(parameters, turns, doctor, &patient))
    {
        return;
    }

    char *specialty = doctor_specialty(doctor);
    cmd2_print_result(patient_name(patient), specialty, hash_turns_specialty_count(turns, specialty));
}

void generate_report(BSTDoctors *doctors, char **parameters)
{
    if (!cm3_error_handler(parameters) || 
        !verify_doctor_existance(doctors, parameters))
    {
        return;
    }
    execute_cmd3(doctors, parameters[0], parameters[1]);
}

static bool cmd1_error_handler(char** parameters, HashTurns *turns, HashPatients *patients)
{
    bool ok = true;

    size_t parameters_count = 1;
    for (size_t i = 0; parameters[i] != NULL; i++)
    {
        parameters_count++;
    }
    if (parameters_count != 3)
    {
        printf("ERROR: cantidad de parametros invalidos para comando '%s'\n", "PEDIR_TURNO");
        ok = false;
    }

    if (!hash_patients_exists(patients, parameters[0]))
    {
        printf("ERROR: no existe le paciente '%s'\n", parameters[0]);
        ok = false;
    }
    if (!hash_turns_specialty_exists(turns, parameters[1]))
    {
        printf("ERROR: no existe la especialidad '%s'\n", parameters[1]);
        ok = false;
    }
    if ((strcmp(parameters[2], "URGENTE") != 0 ) && (strcmp(parameters[2], "REGULAR") != 0 ))
    {
        printf("ERROR: grado de urgencia no identificado ('%s')\n", parameters[2]);
        ok = false;
    }

    return ok;
}

static void cmd1_print_result(char *patient_name, char *specialty, size_t n)
{
    printf("Paciente %s encolado\n", patient_name);
    printf("%ld paciente(s) en espera para %s\n", n, specialty);
}

static bool ask_turn(char** parameters, HashTurns *turns, HashPatients *patients)
{
    return hash_turns_add_turn(turns, parameters[2], parameters[1], hash_patients_get(patients, parameters[0]));
}

static bool cmd2_error_handler(char** parameters, HashTurns *turns, BSTDoctors *doctors)
{
    bool ok = true;

    size_t parameters_count = 1;
    for (size_t i = 0; parameters[i] != NULL; i++)
    {
        parameters_count++;
    }
    if (parameters_count != 1)
    {
        printf("ERROR: cantidad de parametros invalidos para comando '%s'\n", "ATENDER_SIGUIENTE");
        ok = false;
    }

    if (bst_doctors_get_doctor(doctors, parameters[0]) == NULL)
    {
        printf("ERROR: no existe el doctor '%s'\n", parameters[0]);
        ok = false;
    }

    return ok;
}

static void cmd2_print_result(char *name, char *specialty, size_t n)
{
    printf("Se atiende a %s\n", name);
    printf("%ld paciente(s) en espera para %s\n", n, specialty);
}

static bool _attend_patient(char** parameters, HashTurns *turns, Doctor *doctor, Patient **patient)
{
    char *specialty = doctor_specialty(doctor);
    *patient = hash_turns_attend_patient(turns, doctor, specialty);

    if (*patient == NULL)
    {
        printf("No hay pacientes en espera");
        return false;
    }

    return true;
}

static bool cm3_error_handler(char **parameters)
{
    size_t param_count;
    for (param_count = 1; parameters[param_count-1] != NULL; param_count++);

    if (param_count != 2)
    {
        printf(ERROR_CMD_PARAMS_COUNT, "INFORME");
        return false;
    }

    return true;
}

static bool verify_doctor_existance(BSTDoctors *doctors, char **parameters)
{
    Doctor *min = bst_doctors_get_doctor(doctors, parameters[0]);
    Doctor *max = bst_doctors_get_doctor(doctors, parameters[1]);

    if (min == NULL || max == NULL)
    {
        printf(ERROR_DOCTOR, min == NULL ? parameters[0] : parameters[1]);
        return false;
    }

    return true;
}

static bool visit_doctors_in_range(const char *clave, void *dato, void *extra)
{
    Report *report = extra;
    if (strcmp(clave, report_min(report)) < 0 || strcmp(clave, report_max(report)) > 0)
    {
        return false;
    }
    Doctor *doctor = dato;
    printf(DOCTOR_REPORT, report_get_count(report), doctor_name(doctor), doctor_specialty(doctor), doctor_attended_patients(doctor));
    report_count_increment(report);
    return true;
}

static void execute_cmd3(BSTDoctors *doctors, const char *min, const char *max)
{
    Report *doctors_report;

    if ((doctors_report = report_create(min, max)) == NULL)
    {
        printf(ERROR_MEM, "doctors_report");
        return;
    }

    bst_doctors_in_order(doctors, visit_doctors_in_range, doctors_report);

    report_destroy(doctors_report);
}