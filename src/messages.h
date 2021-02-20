#ifndef __MENSAJES__H_
#define __MENSAJES__H_

#define PATIENT_ENQUEUED "Paciente %s encolado\n"
#define ENQUEUED_PATIENT_COUNT "%zu paciente(s) en espera para %s\n"
#define ERR_PATIENT "ERROR: no existe el paciente '%s'\n"
#define ERR_SPECIALTY "ERROR: no existe la especialidad '%s'\n"
#define ERR_URGENCY "Error: grado de urgencia no identificado ('%s')\n"

#define PATIENT_ATTENDED "Se atiende a %s\n"
#define NO_PATIENTS "No hay pacientes en espera\n"
#define ERR_DOCTOR "ERROR: no existe el doctor '%s'\n"

#define DOCTORES_SISTEMA "%zu doctor(es) en el sistema\n"
#define INFORME_DOCTOR "%zu: %s, especialidad %s, %zu paciente(s) atendido(s)\n"

#define ERR_PARAMS_COUNT "No se recibieron los 2 (dos) parametros: <archivo doctores> y <archivo pacientes>\n"
#define ERR_FILE "No se pudo leer archivo %s\n"
#define ERR_YEAR "Valor no numerico en campo de anio: %s\n"
#define ERR_FORMAT "ERROR: formato de comando incorrecto ('%s')\n"
#define ERR_CMD "ERROR: no existe el comando '%s'\n"

#define ERR_CMD_PARAMS_COUNT "ERROR: cantidad de parametros invalidos para comando '%s'\n"

#endif
