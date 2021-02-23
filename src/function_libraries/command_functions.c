#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
/* Inclución de librerías de mensajes */
#include "../message_libraries/error_messages.h"
#include "../message_libraries/success_messages.h"
/* Inclución de estructuras */
#include "../our_tda/doctor/doctor.h"
#include "../our_tda/command/report.h"
#include "../our_tda/doctor/bst_doctors.h"
#include "../our_tda/patient/hash_patients.h"
#include "../our_tda/turns/hash_turns.h"

/* Firma de funciones auxiliares */

/* Funciones auxiliares para el comando 3 */
static bool handle_cmd3_params_error(char **parameters);
static bool verify_doctor_existance(BSTDoctors *doctors, char **parameters);
static bool visit_doctors_in_range(const char *clave, void *dato, Report *extra);
static void execute_cmd_3(const BSTDoctors *doctors, const char *min, const char *max);

void generate_report(BSTDoctors *doctors, char **parameters)
{
    if (!handle_cmd_3_params_error(parameters) || 
        !verify_doctor_existance(doctors, parameters))
    {
        return;
    }
    execute_cmd_3(doctors, parameters[0], parameters[1]);
}

static bool handle_cmd3_params_error(char **parameters)
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
        printf(ERROR_DOCTOR, min == NULL ? min : max);
        return false;
    }

    return true;
}

static bool visit_doctors_in_range(const char *clave, void *dato, Report *extra)
{
    if (strcmp(clave, report_min(extra)) < 0 || strcmp(clave, report_max(extra)) > 0)
    {
        return false;
    }
    Doctor *doctor = dato;
    printf(DOCTOR_REPORT, report_get_count(extra), doctor_name(doctor), doctor_specialty(doctor), doctor_attended_patients(doctor));
    report_count_increment(extra);
    return true;
}

static void execute_cmd_3(const BSTDoctors *doctors, const char *min, const char *max)
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