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

#define CHR_255 (char)255

/* Firma de funciones auxiliares */
static bool cmd1_error_handler(char** parameters, HashTurns *turns, HashPatients *patients);
static void cmd1_print_result(char *patient_name, char *specialty, size_t n);
static bool ask_turn(char** parameters, HashTurns *turns, HashPatients *patients);
/* Funciones auxiliares para el comando 2 */
static bool cmd2_error_handler(char** parameters, HashTurns *turns, BSTDoctors *doctors);
static void cmd2_print_result(char *patient_name, char *specialty, size_t n);
static bool _attend_patient(char** parameters, HashTurns *turns, Doctor *doctors, Patient **patient);
/* Funciones auxiliares para el comando 3 */
static bool visit_doctors_in_range(const char *clave, void *dato, void *extra);
static void execute_cmd3(BSTDoctors *doctors, const char *min, const char *max);
static void print_report(BSTDoctors *doctors, const char *min, const char *max);
static void pre_doctor_countint(BSTDoctors *doctors, const char *min, const char *max);

static bool parameters_error_handler(char **parameters, char *cmd, size_t param_limit);

void make_appointment(HashTurns *turns, HashPatients *patients, char** parameters)
{
    if (!cmd1_error_handler(parameters, turns, patients) || 
        !ask_turn(parameters, turns, patients))
    {
        return;
    } // O(p + p[2]) + (urgente ? O(1) : O(log n))

    cmd1_print_result(parameters[0], parameters[1], hash_turns_specialty_count(turns, parameters[1])-1); // O(1).
} // (urgentes ? O(1) : O(log n)).

void attend_patient(HashTurns *turns, BSTDoctors *doctors, char** parameters)
{
    if (!cmd2_error_handler(parameters, turns, doctors))
    {
        return;
    } // O(log d).

    Doctor *doctor = bst_doctors_get_doctor(doctors, parameters[0]); // O(log d)
    Patient *patient;
    if (!_attend_patient(parameters, turns, doctor, &patient))
    {
        return;
    } // O(1) si desencola de urgentes, O(log r) si desencola de regulares.

    char *specialty = doctor_specialty(doctor); // O(1).
    cmd2_print_result(patient_name(patient), specialty, hash_turns_specialty_count(turns, specialty)); // O(1).
} // O(log d) urgentes, O(log d + log r) regulares.

void generate_report(BSTDoctors *doctors, char **parameters)
{
    if (!parameters_error_handler(parameters, "INFORME", 2))
    {
        return;
    } // O(param)

    execute_cmd3(doctors, parameters[0], parameters[1]); // O(log d) promedio, O(d) si el rango corresponde a la totalidad de doctores.
}

static bool cmd1_error_handler(char** parameters, HashTurns *turns, HashPatients *patients)
{
    bool ok = true;

    if(!parameters_error_handler(parameters, "PEDIR_TURNO", 3))
    {
        return false;
    } // O(p).

    if (!hash_patients_exists(patients, parameters[0]))
    {
        printf(ERROR_PATIENT, parameters[0]);
        ok = false;
    } // O(1).
    if (!hash_turns_specialty_exists(turns, parameters[1]))
    {
        printf("ERROR: no existe la especialidad '%s'\n", parameters[1]);
        ok = false;
    } // O(1).
    if ((strcmp(parameters[2], "URGENTE") != 0 ) && (strcmp(parameters[2], "REGULAR") != 0 ))
    {
        printf("ERROR: grado de urgencia no identificado ('%s')\n", parameters[2]);
        ok = false;
    } // O(p[2]).

    return ok;
} // O(p + p[2]).

static void cmd1_print_result(char *patient_name, char *specialty, size_t n)
{
    printf("Paciente %s encolado\n", patient_name);
    printf("%ld paciente(s) en espera para %s\n", n+1, specialty);
} // O(1).

static bool ask_turn(char** parameters, HashTurns *turns, HashPatients *patients)
{
    return hash_turns_add_turn(turns, parameters[2], parameters[1], hash_patients_get(patients, parameters[0]));
} // O(1).

static bool cmd2_error_handler(char** parameters, HashTurns *turns, BSTDoctors *doctors)
{
    bool ok = true;

    if(!parameters_error_handler(parameters, "ATENDER_SIGUIENTE", 1))
    {
        return false;
    } // O(param)


    if (bst_doctors_get_doctor(doctors, parameters[0]) == NULL)
    {
        printf(ERROR_DOCTOR, parameters[0]);
        ok = false;
    } // O(1) + O(log d) = O(log d)

    return ok;
} // O(log d)

static void cmd2_print_result(char *name, char *specialty, size_t n)
{
    printf(PATIENT_ATTENDED, name);
    printf(ENQUEUED_PATIENT_COUNT, n, specialty);
} // O(1)

static bool _attend_patient(char** parameters, HashTurns *turns, Doctor *doctor, Patient **patient)
{
    char *specialty = doctor_specialty(doctor); // O(1)
    *patient = hash_turns_attend_patient(turns, doctor, specialty); // O(1) si desencola de urgentes, O(log d) si desencola de regulares.

    if (*patient == NULL)
    {
        printf(NO_PATIENTS);
        return false;
    } // O(1)

    return true;
} // O(1) si desencola de urgentes, O(log d) si desencola de regulares.

static bool parameters_error_handler(char **parameters, char *cmd, size_t param_limit)
{
    size_t param_count;
    for (param_count = 0; parameters[param_count] != NULL; param_count++);
    if (param_count != param_limit)
    {
        printf(ERROR_CMD_PARAMS_COUNT, cmd);
        return false;
    } // O(1)

    return true;
} // O(param)

static bool visit_doctors_in_range(const char *clave, void *dato, void *extra)
{
    Report *report = extra;
    if (strcmp(clave, report_max(report)) > 0)
    {
        return false;
    }
    else if (strcmp(clave, report_min(report)) >= 0)
    {
        Doctor *doctor = dato;
        printf(DOCTOR_REPORT, report_get_count(report)+1, doctor_name(doctor), doctor_specialty(doctor), doctor_attended_patients(doctor));
        report_count_increment(report);
    }
    return true;
}
static bool visit_count_doctors(const char *clave, void *dato, void *extra)
{
    Report *report = extra;
    if (strcmp(clave, report_max(report)) > 0)
    {
        return false;
    }
    else if (strcmp(clave, report_min(report)) >= 0)
    {
        report_count_increment(report);
    }
    return true;
}

static void pre_doctor_countint(BSTDoctors *doctors, const char *min, const char *max)
{
    Report *doctor_counter_report;
    if ((doctor_counter_report = report_create(min, max)) == NULL)
    {
        printf(ERROR_MEM, "doctor_counter_report");
        return;
    }
    bst_doctors_in_order(doctors, visit_count_doctors, doctor_counter_report);
    printf(DOCTOR_COUNT, report_get_count(doctor_counter_report));
    report_destroy(doctor_counter_report);
}

static void print_report(BSTDoctors *doctors, const char *min, const char *max)
{
    Report *doctor_print_report;
    if ((doctor_print_report = report_create(min, max)) == NULL)
    {
        printf(ERROR_MEM, "doctor_print_report");
        return;
    }
    bst_doctors_in_order(doctors, visit_doctors_in_range, doctor_print_report);
    report_destroy(doctor_print_report);
}

static void execute_cmd3(BSTDoctors *doctors, const char *min, const char *max)
{
    pre_doctor_countint(doctors, min, max); // O(log d)
    print_report(doctors, min, max); // O(log d)
}