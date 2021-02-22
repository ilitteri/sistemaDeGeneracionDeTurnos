#include <stdlib.h>

#include "hash.h"
#include "cola.h"
#include "queue_patients.h"
#include "heap_patients.h"

typedef struct Hash HashTurns;

// typedef queue_patients_destroy hash_urgent_destroy;
// typedef heap_patients_destroy hash_regulars_destroy;

HashTurns *hash_turns_create();

bool hash_turns_add_turn(HashTurns *turns, Priority urgency, char *specialty, char *patient_name);

bool hash_turns_add_specialty(HashTurns *turns, char *specialty);

bool hash_turns_attend_patient(HashTurns *turns, char *specialty);

void hash_turns_destroy(HashTurns *turns);