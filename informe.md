# Informe TP2: Sistema de Generación de Turnos e Informes Clínicos

## Tabla de Contenidos

- [Datos Personales del Grupo](#Datos-Personales-del-Grupo)
    - [Integrantes](#Integrantes)
    - [Ayudante Asignado](#Ayudante-Asignado)
- [Análisis y Diseño (pre-escritura)](#Análisis-y-Diseño-(pre-escritura))
    - [Fase de iniciación](#Fase-de-iniciación)
        - [Primera entrada del programa](#1.-Primera-entrada-del-programa)
        - [Manejo de errores de la primera entrada del programa](#2.-Manejo-de-errores-de-la-primera-entrada-del-programa)
        - [Lectura de CSV](#3.-Lectura-de-CSV)
        - [Manejo de errores de datos de archivos CSV](#4.-Manejo-de-errores-de-datos-de-archivos-CSV)
    - [Fase de asignación de datos a estructuras](#Fase-de-asignación-de-datos-a-estructuras)
        - [Almacenamiento de datos del archivo CSV de doctores](#5.-Almacenamiento-de-datos-del-archivo-CSV-de-doctores)
        - [Almacenamiento de datos del archivo CSV de pacientes](#6.-Almacenamiento-de-datos-del-archivo-CSV-de-pacientes)
    - [Fase de comandos](#Fase-de-comandos)
    - [Fase de salida](#Fase-de-salida)


|  | [Estructuras de Datos](#Estructuras-de-Datos) |  |
| --- | --- | --- |
| [Doctor](#Doctor)  | [Paciente](#Paciente)  | [Árbol de doctores](#Árbol-de-doctores)  |
| [Hash de pacientes](#Hash-de-pacientes)  | [Cola de Pacientes](#Cola-de-Pacientes)  | [Heap de Pacientes](#Heap-de-Pacientes)  |
| [Hash de turnos](#Hash-de-turnos)  | [Hash de turnos](#Hash-de-turnos)  | [Reporte](#Reporte)  |

|  | [Código del programa](#Código-del-programa) |  |
| --- | --- | --- |
| [main.c](#main) | [load_structures.c](#load-structures) | [command_functions.c](#command-functions) |
| [csv.c](#csv) | [error_messages.h](#error_messages) | [success_messages.h](#success_messages) |

# Datos Personales del Grupo
[*Indice*](#Tabla-de-Contenidos)

## Integrantes

- Millares Faustino.
- Litteri Iván.

## Ayudante Asignado

- Ezequiel Genender

# Análisis y Diseño (pre-escritura)

## Fase de iniciación 
[*Indice*](#Tabla-de-Contenidos)

### 1. Primera entrada del programa

El comando de ejecución del proyecto incluye 2 parámetros. Ambos son nombres de archivos CSV, el primero corresponde a el archivo CSV de doctores, el segundo corresponde a un archivo CSV de pacientes.

### 2. Manejo de errores de la primera entrada del programa

Se manejan los posibles errores que pueden llegar a aparecer, entre ellos:
- que se reciban menos, o más de dos parámetros,
- que alguno de los archivos no se puedan leer.

En caso de que alguno de estoy ocurra, la ejecución del programa se aborta con un exit con errno 1.

### 3. Lectura de CSV

Se leen los CSV, y se guardan las líneas leídas en una lista (una para cada archivo).

### 4. Manejo de errores de datos de archivos CSV

Hay que manejar los posibles errores que pueden llegar a aparecer luego de la lectura, que pueden deberse a:
- algún año no es un valor numérico.

## Fase de asignación de datos a estructuras 
[*Indice*](#Tabla-de-Contenidos)

### 5. Almacenamiento de datos del archivo CSV de doctores

Se tiene que recorrer la lista de líneas del archivo de doctores, parsear a las mismas, y en este caso utilizaremos un struct para cada doctor, la que contendrá la información de su especialidad y la cantidad de pacientes que atendió.

Cada uno de estos doctores, se almacenará en una estructura de *árbol binario de búsqueda*, para cumplir con la complejidad pedida para el *comando informe doctores*, que debe ser *O(d)* en el peor caso (en el que se tenga que mostrar todos los doctores del sistema), *O(log d)* en un caso promedio (en el caso en el que no se pidan mostrar demasiados doctores). Y la estructura elegida cumple con esto. Las claves en dicha estructura corresponderán a el nombre de doctor, y su valor será la estructura mencionada en el párrafo anterior.

### 6. Almacenamiento de datos del archivo CSV de pacientes

Se tiene que recorrer la lista de líneas del archivo de pacientes, parsear a la misma, y en este caso no utilizaremos un struct para cada paciente, ya que la única información que disponemos y necesitaremos de los pacientes es su año de ingreso en la clínica.

Usaremos una estructura *hash* para almacenar los datos de los pacientes aprovechando su búsqueda de *O(1)* (para que no interfiera con la complejidad pedida en *pedir turno*). Donde la clave será el nombre del paciente y el valor el año de ingreso (a menos que necesitemos en algún momento la especialidad que requiere entonces pasaremos a una estructura).

De esta forma, cuando en *pedir turno* se acceda a estos datos, la búsqueda de pacientes será en *O(1)* y más adelante se explicará la estructura utilizada para guardar los turnos, la cuál (adelanto), involucrará una búsqueda en *O(1)* y un guardado en *O(1)* para casos *urgentes* y un guardado *O(log n)* para casos *regulares*. 

## Fase de comandos
[*Indice*](#Tabla-de-Contenidos)

El programa espera al ingreso de comandos. En esta instancia solo 3 tipos de comandos son válidos. 

Uno que corresponde a el pedido de turnos. Este consta de 3 parámetros que debemos verificar que sea válido (paciente existente, especialidad disponible, y urgencia válida). 

Vamos a usar un hash para los turnos urgentes y un hash para lo turnos regulares. Casa hash tendrá como clave la especialidad y cada especialidad como valor tendrá, una cola de espera ordenada por orden de llegada para los urgentes; y un heap ordenado por año de ingreso y orden de llegada en caso de tener el mismo año de ingreso para los regulares. 

## Fase de salida
[*Indice*](#Tabla-de-Contenidos)

Manejo de errores y devolución de la fase la buena fase. 

# Estructuras de datos

## [Doctor](src/our_tda/doctor/doctor.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Doctor](src/our_tda/doctor/doctor.h)
```c
typedef struct
{
    char *name;
    char *specialty;
    int attended_patients;
} Doctor;
```
### [Primitivas Doctor](src/our_tda/doctor/doctor.h)

```c
/* Registra un doctor, 
*  recibe el nombre 
*  y la especialidad
*/
Doctor *doctor_check_in(char *name, char *specialty);

/* Obtiene el nombre del doctor 
*  Pre: el doctor fue registrado
*  Post: devuelve nombre de doctor
*/
char *doctor_name(const Doctor *doctor);

/* Obtiene la especialidad del doctor 
*  Pre: el doctor fue registrado
*  Post: devuelve la especialidad de doctor
*/
char *doctor_specialty(const Doctor *doctor);

/* Obtiene la cantidad de pacientes atendidos por el doctor
*  Pre: el doctor fue registrado
*  Post: devuelve la cantidad de pacientes atendidos por el doctor
*/
size_t doctor_attended_patients(const Doctor *doctor);

/* Suma 1 a la cantidad de pacientes atendidos por el doctor
*  Pre: el doctor fue registrado
*  Post: la cantidad de pacientes es atendidos aumento 1
*/
void doctor_attend_patient(Doctor *doctor);

/* Destruye los datos del doctor */
void doctor_destroy(Doctor *doctor);
```

## [Paciente](src/our_tda/patient/patient.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Paciente]((src/our_tda/patient/patient.h))
```c
typedef struct
{
    char *name;
    int entry_year;
} Patient;
```

### [Primitivas Paciente]((src/our_tda/patient/patient.h))
```c
/* Registra un paciente */
Patient *patient_check_in(char *name, size_t year);

/*  Obtiene el nombre de el paciente (no modifica al paciente)
*   Pre: el paciente fué registrado.
*   Pos: nombre del paciente.
*/
char *patient_name(const Patient *patient);

/*  Obtiene el anio de entrada del paciente (no modifica al paciente)
*   Pre: el paciente fué registrado.
*   Pos: anio de registro del paciente.
*/
size_t patient_entry_year(const Patient *patient);

/* Destruye los datos del paciente */
void destroy_patient(Patient *patient);
```

## [Árbol de doctores](stc/out_tda/doctor/bst_doctors.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Árbol de Doctores](stc/out_tda/doctor/bst_doctors.h)
```c
typedef abb_t BSTDoctors;
```

### [Funciones Árbol de Doctores](stc/out_tda/doctor/bst_doctors.h)
```c
typedef abb_comparar_clave_t bst_key_cmp;
typedef abb_destruir_dato_t bst_doctor_destroy;
```
### [Primitivas Árbol de Doctores](stc/out_tda/doctor/bst_doctors.h)
```c
/* Crea la estructura */
BSTDoctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data);

/* Guarda un doctor en la estrucutra
*  Pre: la estrucutra fue creada, se registró al doctor.
*  Pos: se guarda al doctor en la estructura.
*/
bool bst_doctors_save_doctor(BSTDoctors *doctors, const char *doctor_name, Doctor *doctor);

/* Devuelve el struct doctor correspondiente al nombre de este.
*  Pre: la estrucutra fue creada, se registró al doctor y se guardó en la estructura.
*  Pos: se devulve el doctor correspondiente.
*/
Doctor *bst_doctors_get_doctor(const BSTDoctors *doctors, const char *doctor_name);

/* Devuelve el struct doctor correspondiente al nombre de este.
*  Pre: la estrucutra fue creada, se registró al doctor y se guardó en la estructura.
*  Pos: se devulve la cantidad de doctores registrados.
*/
size_t bst_doctors_count(BSTDoctors *doctors);

/* Destruye la estructura y sus datos si bst_doctor_destroy es distinto de NULL */
void bst_doctors_destroy(BSTDoctors *doctors);

/* Implementa el iterador interno, que recorre el arbol in-order.
   "visitar" es una función de callback que recibe la clave, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario).
 */
void bst_doctors_in_order(BSTDoctors *doctors,
                  bool visit(const char *, void *, void *),
                  Report *report);
```

## [Hash de pacientes](src/our_tda/patient/hash_patients.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Hash Pacientes](src/our_tda/patient/hash_patients.h)
```c
typedef hash_t HashPatients;
```

### [Primitivas Hash Pacientes](src/our_tda/patient/hash_patients.h)
```c
/* Crea la estructura */
HashPatients *hash_patients_create(hash_destroy_patient patient_destroy);

/*  Guarda un paciente en la estrucutra
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: devuelve si el paciente se registró correctamente.
*/
bool hash_patients_save(HashPatients *patients, Patient *patient);

/*  Informa si el paciente está registrado
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: devuelve si el paciente estaba registrado anteriormente.
*/
bool hash_patients_exists(const HashPatients *patients, const char *name);

/*  Devuelve un paciente
*   Pre: la estrucutra fue creada, se registró al paciente.
*/
Patient *hash_patients_get(const HashPatients *patients, const char *name);

/* Destruye la estrucura y sus datos en caso de existir remanentes */
void hash_patients_destroy(HashPatients *patients);
```

## [Cola de Pacientes](src/our_tda/turns/queue_patients.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Cola de Pacientes](src/our_tda/turns/queue_patients.h)
```c
typedef struct
{
    cola_t patients;
    size_t cant;
} QueuePatients;
```

### [Primitivas Cola de Pacientes](src/our_tda/turns/queue_patients.h)
```c
/* Crea la estructura */
QueuePatients *queue_patients_create();

/*  Destruye la estrucutura .
*   Pre: la estructura fue creada.
*/
void queue_patients_destroy(QueuePatients *patients, void (*patients_destroy)(void *));

/*  Informa si la cola se encuentra vacía (no modifica a la cola).
*   Pre: la estructura fue creada.
*/
bool queue_patients_is_empty(const QueuePatients *patients);

/*  Encola un paciente.
*   Pre: la estructura fue creada.
*/
bool queue_patients_enqueue(QueuePatients *patients, Patient *patient);

/*  Devuelve la información del primero en la cola, sin modificarla.
*   Pre: la estrucura fue creada.
*/
Patient *queue_patients_first(const QueuePatients *patients);

/*  Desencola un paciente.
*   Pre: la estructura fue creada.
*/
Patient *queue_patients_dequeue(QueuePatients *patients);

/*  Informa la cantidad de pacientes en espera.
*   Pre: la estructura fue creada.
*/
size_t queue_patients_count(QueuePatients *urgent);
```

## [Hash de turnos](src/our_tda/turns/hash_turns.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Hash Turnos](src/our_tda/turns/hash_turns.h)
```c
typedef hash_t Hash_Turns;
```

### [Primitivas Hash Turnos](src/our_tda/turns/hash_turns.h)
```c
/* Crea la estructura */
HashTurns *hash_turns_create();

/*  Agrega un paciente a la cola de espera (dependiendo de la urgencia, va a
*   una u otra).
*   Pre: la estructura HashTurns fué creada.
*/
bool hash_turns_add_turn(HashTurns *turns, char* urgency, char *specialty, Patient *patient);

/*  Agrega una especialidad a el diccionario de turnos urgentes y regulares.
*   Pre: la estructura HashTurns fué creada.
*/
bool hash_turns_add_specialty(HashTurns *turns, char *specialty);

/*  Atiende al siguiente paciente en espera.
*   Pre: la estructura HashTurns fué creada, la especialidad existe, y el
*       doctor está especializado en ella.
*   Pos: datos del paciente desencolado.
*/
Patient *hash_turns_attend_patient(HashTurns *turns, Doctor *doctor, char *specialty);

/*  Informa si existe una determinada especialidad.
*   Pre: la estructura HashTurns fué creada, la especialidad existe, y el doctor
*       está especializado en ella.
*   Pos: true si existe, false si no.
*/
bool hash_turns_specialty_exists(HashTurns *turns, char *specialty);

/*  Informa la cantidad de pacientes en espera de una especialidad.
*   Pre: la estructura HashTurns fué creada.
*/
size_t hash_turns_specialty_count(HashTurns *turns, char *specialty);

/*  Destruye la estructura */
void hash_turns_destroy(HashTurns *turns);
```

## [Heap de Pacientes](src/our_tda/turns/heap_patients.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Heap de Pacientes](src/our_tda/turns/heap_patients.h)
```c
typedef heap_t Heap_Turns;
```

### [Primitivas Heap de Pacientes](src/our_tda/turns/heap_patients.h)
```c
/*  Crea la estrucutra */
HeapPatients *heap_patients_create(HeapPatients_cmp cmp);

/*  Destruye la estrucutra, y si su función de destrucción no es NULL, también
*   sus datos.
*   Pre: la estructura fué creada.
*   Pos: se libera la memoria pedida para la estructura.
*/
void heap_patients_destroy(HeapPatients *turns, void (*patient_destroy) (void *));

/*  Informa la cantidad de pacientes en espera (no modifica la estructura).
*   Pre: la estructura fue creada.
*/
size_t heap_patients_count(const HeapPatients *turns);

/*  Encola un paciente.
*   Pre: la estructura fué creada.
*   Pos: devuelve si el paciente se encoló correctamente.
*/
bool heap_patients_enqueue(HeapPatients *turns, Patient *patient);

/*  Desencola un paciente.
*   Pre: la estructura fué creada.
*   Pos: devuelve el paciente desencolado.
*/  
Patient *heap_patients_dequeue(HeapPatients *turns);
```

## [Reporte](src/our_tda/command/report.c)
[*Indice*](#Tabla-de-Contenidos)

### Descripción

### [Struct Report](src/our_tda/command/report.h)
```c
typedef struct Report Report;
```
### [Primitivas reporte](src/our_tda/command/report.h)
```c
/* Crea la estructura */
Report *report_create(const char *min, const char *max);

/*  Devuelve la cota inferior del recorrido del informe.
*   Pre: la estructura fué creada.
*   Pos: nombre o letra que marca límite inferior de la búsqueda, si es vacío,
*       se guarda "`" como máximo caracter (uno más grande que la "z" en ASCII).
*/
char *report_min(const Report *report);

/*  Devuelve la cota superior del recorrido del informe.
*   Pre: la estructura fué creada.
*   Pos: nombre o letra que marca límite superior de la búsqueda, si es vacío,
*       se guarda "{" como máximo caracter (uno más grande que la "z" en ASCII).
*/
char *report_max(const Report *report);

/*  Informa la cantidad de doctores incluídos en el informe 
*   Pre: la estructura fué creada.
*/
size_t report_get_count(const Report *report);

/*  Incrementa en uno el contador interno de la estructura.
*   Pre: la estructura fué creada.
*/
void report_count_increment(Report *report);

/* Destruye la estructura */
void report_destroy(Report *report);
```

# Código del programa
[*Indice*](#Tabla-de-Contenidos)

## [main.c](main.c)

### Descripción

### Funciones

## [load_structures.c](src/function_libraries/load_structures.c)

### Descripción

### Funciones

## [command_functions.c](src/function_libraries/command_functions.c)

### Descripción

### Funciones

## [csv.c](csv.c)

### Descripción

### Funciones

## [error_messages.h](src/message_libraries/error_messages.h)

### Descripción

## [success_messages.h](src/message_libraries/success_messages.h)

### Descripción