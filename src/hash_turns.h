#include <stdlib.h>

#include "hash.h"
#include "cola.h"

typedef struct HashTurns;

HashTurns *hash_turns_create();

bool hash_turns_add_turn(HashTurns *turns, Priority urgency, char *specialty, char *patient_name);

bool hash_turns_add_specialty(HashTurns *turns, char *specialty);

void hash_turns_destroy(HashTurns *turns);