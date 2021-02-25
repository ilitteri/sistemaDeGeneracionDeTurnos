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
#include "../our_tda/command/range.h"
#include "../our_tda/doctor/bst_doctors.h"
#include "../our_tda/patient/hash_patients.h"
#include "../our_tda/turns/turns_register.h"

/* Firma de funciones auxiliares */
static bool cmd1_error_handler(char** parameters, TurnsRegister *turns, HashPatients *patients);
static void cmd1_print_result(char *patient_name, char *specialty, size_t n);
/* Funciones auxiliares para el comando 2 */
static bool cmd2_error_handler(char** parameters, TurnsRegister *turns, BSTDoctors *doctors);
static void cmd2_print_result(char *patient_name, char *specialty, size_t n);
static bool _attend_patient(char** parameters, TurnsRegister *turns, Doctor *doctors, Patient **patient);
/* Funciones auxiliares para el comando 3 */
static bool visit_doctors_in_range(const char *key, void *data, void *extra);
static bool visit_count_doctors(const char *key, void *data, void *extra);
static bool pre_walk_doctor_count(BSTDoctors *doctors, char **parameters);
/* Función auxiliar para el manejo de errores */
static bool parameters_error_handler(char **parameters, char *cmd, size_t param_limit);

void make_appointment(TurnsRegister *turns, HashPatients *patients, char** parameters)
{
    if (!cmd1_error_handler(parameters, turns, patients) || 
        !turns_register_add_turn(turns, parameters[2], parameters[1], hash_patients_get(patients, parameters[0])))
    {
        return;
    } // O(p + p[2]) + (urgente ? O(1) : O(log n))

    cmd1_print_result(parameters[0], parameters[1], turns_register_specialty_count(turns, parameters[1])-1); // O(1).
} // (urgentes ? O(1) : O(log n)).

void attend_patient(TurnsRegister *turns, BSTDoctors *doctors, char** parameters)
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

    char *specialty = doctor_specialty(doctor);

    cmd2_print_result(patient_name(patient), specialty, turns_register_specialty_count(turns, specialty)); // O(1).
} // O(log d) urgentes, O(log d + log r) regulares.

void generate_report(BSTDoctors *doctors, char **parameters)
{
    if (!parameters_error_handler(parameters, "INFORME", 2))
    {
        return;
    } // O(param)

    size_t doctors_count = bst_doctors_count(doctors);

    if (doctors_count == 0)
    {
        printf(DOCTOR_COUNT, doctors_count);
        return;
    }

    if (pre_walk_doctor_count(doctors, parameters))
    {
        Range *range = range_create(parameters[0], parameters[1]);
        bst_doctors_in_range(doctors, visit_doctors_in_range, range);
        range_destroy(range);
    }
}

static bool cmd1_error_handler(char** parameters, TurnsRegister *turns, HashPatients *patients)
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
    if (!turns_register_specialty_exists(turns, parameters[1]))
    {
        printf(ERROR_SPECIALTY, parameters[1]);
        ok = false;
    } // O(1).
    if ((strcmp(parameters[2], "URGENTE") != 0 ) && (strcmp(parameters[2], "REGULAR") != 0 ))
    {
        printf(ERROR_URGENCY, parameters[2]);
        ok = false;
    } // O(p[2]).

    return ok;
} // O(p + p[2]).

static void cmd1_print_result(char *patient_name, char *specialty, size_t n)
{
    printf(PATIENT_ENQUEUED, patient_name);
    printf(ENQUEUED_PATIENT_COUNT, n+1, specialty);
} // O(1).

static bool cmd2_error_handler(char** parameters, TurnsRegister *turns, BSTDoctors *doctors)
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

static bool _attend_patient(char** parameters, TurnsRegister *turns, Doctor *doctor, Patient **patient)
{
    *patient = turns_register_attend_patient(turns, doctor, doctor_specialty(doctor)); // O(1) si desencola de urgentes, O(log d) si desencola de regulares.

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

static bool visit_doctors_in_range(const char *key, void *data, void *extra)
{
    Range *range = extra;
    Doctor *doctor = data;
    printf(DOCTOR_REPORT, range_get_count(range)+1, doctor_name(doctor), doctor_specialty(doctor), doctor_attended_patients(doctor));
    range_count_increment(range);
    return true;
}
static bool visit_count_doctors(const char *key, void *data, void *extra)
{
    Range *range = extra;
    range_count_increment(range);
    return true;
}

static bool pre_walk_doctor_count(BSTDoctors *doctors, char **parameters)
{
    Range *range = range_create(parameters[0], parameters[1]);
    bst_doctors_in_range(doctors, visit_count_doctors, range);
    printf(DOCTOR_COUNT, range_get_count(range));
    if (range_get_count(range) == 0)
    {
        range_destroy(range);
        return false;
    }
    range_destroy(range);
    return true;
}