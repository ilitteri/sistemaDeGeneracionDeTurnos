#include <stdlib.h>
#include <string.h>

#include "range.h"

#define MAX_CHAR "z"
#define MIN_CHAR "A"
#define MIN_CHAR_SIZE 2

struct Range
{
    char *min, *max;
    size_t counter;
};

Range *range_create(const char *min, const char *max)
{
    Range *doctors_range;

    if ((doctors_range = malloc(sizeof(Range))) == NULL)
    {
        return NULL;
    }

    size_t min_len = strlen(min);

    if ((doctors_range->min = malloc(min_len == 0 ? MIN_CHAR_SIZE : min_len+1)) == NULL)
        {
            free(doctors_range);
            return NULL;
        }
    strcpy(doctors_range->min, min_len == 0 ? MIN_CHAR : min);

    size_t max_len = strlen(max);

    if ((doctors_range->max = malloc(max_len == 0 ? MIN_CHAR_SIZE : max_len+1)) == NULL)
        {
            free(doctors_range->min);
            free(doctors_range);
            return NULL;
        }
    strcpy(doctors_range->max, max_len == 0 ? MAX_CHAR : max);

    doctors_range->counter = 0;

    return doctors_range;
}

char *range_min(const Range *range)
{
    return range->min;
}

char *range_max(const Range *range)
{
    return range->max;
}

size_t range_get_count(const Range *range)
{
    return range->counter;
}

void range_count_increment(Range *range)
{
    range->counter++;
}

void range_destroy(Range *range)
{
    free(range->min);
    free(range->max);
    free(range);
}