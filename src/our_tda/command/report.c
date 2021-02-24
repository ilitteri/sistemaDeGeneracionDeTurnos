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

    size_t min_len = strlen(min);

    if ((doctors_report->min = malloc(min_len == 0 ? MIN_CHAR_SIZE : min_len+1)) == NULL)
        {
            free(doctors_report);
            return NULL;
        }
    strcpy(doctors_report->min, min_len == 0 ? MIN_CHAR : min);

    size_t max_len = strlen(max);

    if ((doctors_report->max = malloc(max_len == 0 ? MIN_CHAR_SIZE : max_len+1)) == NULL)
        {
            free(doctors_report->min);
            free(doctors_report);
            return NULL;
        }
    strcpy(doctors_report->max, max_len == 0 ? MAX_CHAR : max);

    doctors_report->counter = 0;

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

void report_count_increment(Report *report)
{
    report->counter++;
}

void report_destroy(Report *report)
{
    free(report->min);
    free(report->max);
    free(report);
}