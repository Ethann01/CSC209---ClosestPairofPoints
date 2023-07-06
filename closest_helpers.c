#include "closest_helpers.h"
#include <math.h>

int verbose = 0;

int compare_x(const void* a, const void* b)
{
    return (*(struct Point *)a).x - (*(struct Point *)b).x;
}

int compare_y(const void* a, const void* b)
{
    return (*(struct Point *)a).y - (*(struct Point *)b).y;
}

double dist(struct Point p1, struct Point p2)
{
    return sqrt((pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2)));
}