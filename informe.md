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
- [Estructuras de datos](#Estructuras-de-datos)
    - [Doctor](#Doctor)
        - [Struct Doctor](#Struct-Doctor)
        - [Primitivas Doctor](#Primitivas-Doctor)
    - [Paciente](#Paciente)
        - [Struct](#Struct-Paciente)
        - [Primitivas](#Primitivas-Paciente)
    - [Árbol de doctores](#Árbol-de-doctores)
        - [Struct](#Struct-Árbol-de-Doctores)
        - [Funciones](#Funciones-Árbol-de-Doctores)
        - [Primitivas](#Primitivas-Árbol-de-Doctores)
    - [Hash de pacientes](#Hash-de-pacientes)
        - [Struct](#Struct-Hash-Pacientes)
        - [Funciones](#Funciones-Hash-Pacientes)
        - [Primitivas](#Primitivas-Hash-Pacientes)
    - [Cola de Pacientes](#Cola-de-Pacientes)
        - [Struct](#Struct-Cola-de-Pacientes)
        - [Funciones](#Funciones-Cola-de-Pacientes)
        - [Primitivas](#Primitivas-Cola-de-Pacientes)
    - [Hash de turnos](#Hash-de-turnos)
        - [Struct](#Struct-Hash-Turnos)
        - [Funciones](#Funciones-Hash-Turnos)
        - [Primitivas](#Primitivas-Hash-Turnos)
    - [Heap de Regulares](#Heap-de-Regulares)
        - [Struct](#Struct-Heap-de-Regulares)
        - [Funciones](#Funciones-Heap-de-Regulares)
        - [Primitivas](#Primitivas-Heap-de-Regulares)
- [Código del programa](#Código-del-programa)

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

## Doctor
[*Indice*](#Tabla-de-Contenidos)

### Struct Doctor
```c
typedef struct
{
    char *name;
    char *specialty;
    int attended_patients;
} Doctor;
```
### Primitivas Doctor

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
int doctor_attended_patients(const Doctor *doctor);

/* Suma 1 a la cantidad de pacientes atendidos por el doctor
*  Pre: el doctor fue registrado
*  Post: la cantidad de pacientes es atendidos aumento 1
*/
void doctor_attend_patient(Doctor *doctor);

/* Destruye los datos del doctor */
void doctor_destroy(Doctor *doctor);
```

## Paciente
[*Indice*](#Tabla-de-Contenidos)

### Struct Paciente
```c
typedef struct
{
    char *name;
    int entry_year;
} Patient;
```

### Primitivas Paciente
```c
/* Registra un paciente */
Patient *patient_check_in(char *name, size_t entry_year);

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
```

## Árbol de doctores
[*Indice*](#Tabla-de-Contenidos)

### Struct Árbol de Doctores
```c
typedef abb_t BSTDoctors;
```

### Funciones Árbol de Doctores
```c
typedef abb_comparar_clave_t bst_key_cmp;
typedef abb_destruir_dato_t bst_doctor_destroy;
```
### Primitivas Árbol de Doctores
```c
/* Crea la estructura */
BSTDoctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data);

/* Guarda un doctor en la estrucutra
*  Pre: la estrucutra fue creada, se registró al doctor.
*  Pos: se guarda al doctor en la estructura.
*/
bool bst_doctors_save_doctor(BSTDoctors *doctors, const char *doctor_name, Doctor *doctor);

/* Devuelve el struct doctor correspondiente al nombre de este.
*  Pre: la estrucutra fue creada, se registró al doctor y se guardp en la estructura.
*  Pos: se devulve el doctor correspondiente.
*/
Doctor *bst_doctors_get_doctor(const BSTDoctors *doctors, const char *doctor_name);

/* Destruye la estructura y sus datos si bst_doctor_destroy es distinto de NULL */
void bst_doctors_destroy(BSTDoctors *doctors);
```

## Hash de pacientes
[*Indice*](#Tabla-de-Contenidos)

### Struct Hash Pacientes
```c
typedef hash_t HashPatients;
```

### Primitivas Hash Pacientes
```c
/* Crea la estructura */
HashPatients *hash_patients_create(hash_destroy_patient destroy_data);

/*  Guarda un paciente en la estrucutra
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: se guarda al paciente en la estructura.
*/
bool hash_patients_save(HashPatients *patients, Patient *patient);

/*  Informa si el paciente está registrado
*   Pre: la estrucutra fue creada, se registró al paciente.
*   Pos: el paciente está guardado o no.
*/
bool hash_patients_exists(const HashPatients *patients, const char *name);

/* Destruye la estrucura y sus datos en caso de existir remanentes */
void hash_patients_destroy(HashPatients *patients);
```

## Cola de Pacientes
[*Indice*](#Tabla-de-Contenidos)

### Struct Cola de Pacientes
```c
typedef struct
{
    cola_t patients;
    size_t cant;
} QueuePatients;
```

### Primitivas Cola de Pacientes
```c
/* Crea la estructura */
QueuePatients *queue_patients_create();

/*  Destruye la estrucutura .
*   Pre: la estructura fue creada.
*/
void queue_patients_destroy(QueuePatients *patients, void (*patients_destroy)(Patient *));

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
```

## Hash de turnos
[*Indice*](#Tabla-de-Contenidos)

### Struct Hash Turnos
```c
typedef hash_t Hash_Turns;
```

### Primitivas Hash Turnos
```c
Hash_Turns *hash_turns_create(hash_turn_destroy destroy_data);
bool hash_turns_save(Hash_Turns *turns, const char *specialty, Patient *patient);
bool hash_turns_specialty_exists(const Hash_Turns *turns, const char *specialty);
void hash_turns_destroy(Hash_Turns *turns);
```

## Heap de Regulares
[*Indice*](#Tabla-de-Contenidos)

### Struct Heap de Regulares
```c
typedef heap_t Heap_Turns;
```

### Primitivas Heap de Regulares
```c
Heap_Turns *heap_regulars_create(heap_key_cmp cmp);
void heap_regulars_destroy(Heap_Turns *turns, void (*destroy_turn) (Patient *p));
size_t heap_regulars_count(const Heap_Turns *turns);
bool heap_regulars_enqueue(const Heap_Turns *turns);
Heap_Turns *heap_regulars_dequeue(Heap_Turns *turns); 
```

# Código del programa
[*Indice*](#Tabla-de-Contenidos)