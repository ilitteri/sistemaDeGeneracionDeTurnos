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
    - [Paciente](#Paciente)
    - [Árbol de doctores](#Árbol-de-doctores)
    - [Hash de pacientes](#Hash-de-pacientes)
    - [Cola de prioridad](#Cola-de-prioridad)
    - [Hash de turnos regulares y turnos urgentes](#Hash-de-turnos-regulares-y-turnos-urgentes)
    - [Heap de pacientes con turno regular](#Heap-de-pacientes-con-turno-regular)
- [Código del programa](#Código-del-programa)

# Datos Personales del Grupo
[*Indice*](#Tabla-de-Contenidos)

---

## Integrantes

- Millares Faustino.
- Litteri Iván.

## Ayudante Asignado

- Ezequiel Genender

# Análisis y Diseño (pre-escritura)
---

## Fase de iniciación 
[*Indice*](#Tabla-de-Contenidos)

### 1. Primera entrada del programa

El programa recibe como argumentos dos archivos en formato CSV.

### 2. Manejo de errores de la primera entrada del programa

Hay que manejar los posibles errores que pueden llegar a aparecer, entre ellos:
- que se reciban menos, o más de dos parámetros,
- que alguno de los archivos, o ambos no existan.

### 3. Lectura de CSV

Se leen los CSV, y se guardan las líneas leídas en una lista (una para cada archivo).

### 4. Manejo de errores de datos de archivos CSV

Hay que manejar los posibles errores que pueden llegar a aparecer luego de la lectura, que pueden deberse a:
- algún año no es un valor numérico.

## Fase de asignación de datos a estructuras 
[*Indice*](#Tabla-de-Contenidos)

### 5. Almacenamiento de datos del archivo CSV de doctores

Se tiene que recorrer la lista de líneas del archivo de doctores, parsear a la misma, y en este caso utilizaremos un struct para cada doctor, la que contendrá la información de su especialidad y la cantidad de pacientes que atendió.

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

### Struct
```c
typedef struct
{
    char *name;
    char *specialty;
    int attended_patients;
} Doctor;
```
### Primitivas

```c
char *get_name(Doctor *d)
char *get_specialty(Doctor *d);
int get_attended_patients(Doctor *d);
void attend_patient(Doctor *d);
```

## Paciente
[*Indice*](#Tabla-de-Contenidos)

### Struct 
```c
typedef struct
{
    char *name;
    int entry_year;
} Patient;
```

### Primitivas
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

### Struct
```c
typedef abb_t BST_Doctors;
```

### Primitivas
```c
BST_Doctors *bst_doctors_create(bst_key_cmp cmp, bst_doctor_destroy destroy_data);
bool bst_save_doctor(BST_Doctors *doctors, const char *doctor_name, Doctor *d);
Doctor *bst_remove_doctor(BST_Doctors *doctors, const char *doctor_name);
Doctor *bst_get_doctor(cont BST_Doctors *doctors, const char *doctor_name);
void bst_doctors_destroy(BST_Doctors *doctors);
```

## Hash de pacientes
[*Indice*](#Tabla-de-Contenidos)

### Struct
```c
typedef hash_t Hash_Patients;
```

### Primitivas
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

## Cola de prioridad
[*Indice*](#Tabla-de-Contenidos)

### Struct
```c
typedef cola_t Priority_Queue; // ¿COMO CUENTO LA CANTIDAD DE PACIENTES RESTANTES?
```

### Primitivas
```c
/* Crea la estrucutra */
PriorityQueue *priority_queue_create();

/*  Destruye la estrucutura 
*   Pre: la estructura fue creada.
*/
void priority_queue_destroy(PriorityQueue *queue, void (*destroy_patient) (Patient *patient));

/*  Encola un paciente
*   Pre: la estructura fue creada.
*/
bool enqueue_patient(PriorityQueue *queue, Patient *patient);

/*  Desencola un paciente
*   Pre: la estructura fue creada.
*/
Patient *dequeue_patient(PriorityQueue *queue);
```

## Hash de turnos regulares y turnos urgentes
[*Indice*](#Tabla-de-Contenidos)

### Struct
```c
typedef hash_t Hash_Turns;
```

### Primitivas
```c
Hash_Turns *hash_turns_create(hash_turn_destroy destroy_data);
bool hash_turns_save(Hash_Turns *turns, const char *specialty, Patient *patient);
bool hash_turns_specialty_exists(const Hash_Turns *turns, const char *specialty);
void hash_turns_destroy(Hash_Turns *turns);
```

## Heap de pacientes con turno regular
[*Indice*](#Tabla-de-Contenidos)

### Struct
```c
typedef heap_t Heap_Turns;
```

### Primitivas
```c
Heap_Turns *heap_turns_create(heap_key_cmp cmp);
void heap_turns_destroy(Heap_Turns *turns, void (*destroy_turn) (Patient *p));
size_t heap_turns_count(const Heap_Turns *turns);
bool heap_turns_enqueue(const Heap_Turns *turns);
Heap_Turns *heap_turns_dequeue(Heap_Turns *turns); 
```

# Código del programa
[*Indice*](#Tabla-de-Contenidos)