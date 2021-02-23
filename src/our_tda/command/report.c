#include <stdlib.h>
#include <string.h>

typedef struct Report
{
    char *min, *max;
    size_t count;
} Report;

Report *report_create(const char *min, const char *max)
{
    Report *doctors_report;

    if ((doctors_report = malloc(sizeof(Report))) == NULL)
    {
        return NULL;
    }

    if ((doctors_report->min = malloc(strlen(min)+1)) == NULL)
    {
        free(doctors_report);
        return NULL;
    }

    strcpy(doctors_report->min, min);

    if ((doctors_report->max = malloc(strlen(min)+1)) == NULL)
    {
        free(doctors_report->min);
        free(doctors_report);
        return NULL;
    }

    strcpy(doctors_report->max, max);

    doctors_report->count = 0;

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
    return report->count;
}

void report_count_increment(Report *report)
{
    report->count++;
}

void report_destroy(Report *report)
{
    free(report->min);
    free(report->max);
    free(report);
}