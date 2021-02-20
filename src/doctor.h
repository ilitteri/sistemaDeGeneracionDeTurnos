#include <stdlib.h>

/* Struct */
typedef struct Doctor Doctor;

/* Primitivas de Doctor */

/* Registra un doctor, 
*  recibe el nombre 
*  y la especialidad
*/
Doctor *doctor_check_in(char *name, char *specialty);

/* Obtiene el nombre del doctor 
*  Pre: el doctor fue registrado
*  Post: devuelve nombre de doctor
*/
char *get_name(const Doctor *doctor);

/* Obtiene la especialidad del doctor 
*  Pre: el doctor fue registrado
*  Post: devuelve la especialidad de doctor
*/
char *get_specialty(const Doctor *doctor);

/* Obtiene la cantidad de pacientes atendidos por el doctor
*  Pre: el doctor fue registrado
*  Post: devuelve la cantidad de pacientes atendidos por el doctor
*/
int get_attended_patients(const Doctor *doctor);

/* Suma 1 a la cantidad de pacientes atendidos por el doctor
*  Pre: el doctor fue registrado
*  Post: la cantidad de pacientes es atendidos aumento 1
*/
void attend_patient(Doctor *doctor);

/* Destruye los datos del doctor */
void destroy_doctor(Doctor *doctor);