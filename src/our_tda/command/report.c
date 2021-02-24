#include <stdlib.h>
#include <string.h>

#define MAX_CHAR "z"
#define MIN_CHAR "A"
#define MIN_CHAR_SIZE 2

typedef struct Report
{
    char *min, *max;
    size_t counter;
} Report;

Report *report_create(const char *min, const char *max)
{
    Report *doctors_report;

    if ((doctors_report = malloc(sizeof(Report))) == NULL)
    {
        return NULL;
    }

    if (strlen(min) == 0) {
        if ((doctors_report->min = malloc(MIN_CHAR_SIZE)) == NULL)
        {
            free(doctors_report);
            return NULL;
        }
        // doctors_report->min = MIN_CHAR;
        strcpy(doctors_report->min, MIN_CHAR);
    }
    else
    {
        if ((doctors_report->min = malloc(strlen(min)+1)) == NULL)
        {
            free(doctors_report);
            return NULL;
        }
        strcpy(doctors_report->min, min);
    }

    if (strlen(max) == 0) {
        if ((doctors_report->max = malloc(MIN_CHAR_SIZE)) == NULL)
        {
            free(doctors_report);
            return NULL;
        }
        strcpy(doctors_report->max, MAX_CHAR);
    }
    else
    {
        if ((doctors_report->max = malloc(strlen(max)+1)) == NULL)
        {
            free(doctors_report);
            return NULL;
        }
        strcpy(doctors_report->max, max);
    }

    doctors_report->counter = 0;
    // doctors_report->doctors_count = 0;

    return doctors_report;
}

char *report_min(const Report *report)
{
    return report->min;
}

char *report_max(const Report *report)
{
    return report->max;
}

size_t report_get_count(const Report *report)
{
    return report->counter;
}

// size_t report_get_doctors_count(const Report *report)
// {
//     return report->doctors_count;
// }

void report_count_increment(Report *report)
{
    report->counter++;
}

// void report_doctors_count_increment(Report *report)
// {
//     report->doctors_count++;
// }

void report_destroy(Report *report)
{
    free(report->min);
    free(report->max);
    free(report);
}