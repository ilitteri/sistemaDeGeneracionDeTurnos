#include <stdlib.h>

typedef struct Report Report;

Report *report_create(const char *min, const char *max);

char *report_min(const Report *report);

char *report_max(const Report *report);

size_t report_get_count(const Report *report);

void report_count_increment(Report *report);

void report_doctors_count_increment(Report *report);

bool report_print(const Report *report);

size_t report_get_doctors_count(const Report *report);

void report_destroy(Report *report);