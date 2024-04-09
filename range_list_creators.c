#include <stdio.h>
#include <stdlib.h>

float *centered_rangelist(float list_center, float range, int size)
{
    float *list = malloc(sizeof(float) * size);
    if (list == NULL)
    {
        return NULL;
    }

    float list_start = list_center - (range / 2);
    for (float i = 0, n = range / size, k = n / 2; i < size; i++)
    {
        list[(int)i] = list_start + k;
        k += n;
    }
    return list;
}

float *length_rangelist(float range[2], int length)
{
    if (range[0] >= range[1])
    {
        return NULL;
    }

    float *list = malloc(sizeof(float) * length);
    if (list == NULL)
    {
        return NULL;
    }

    float range_diff = range[1] - range[0];
    float increase = range_diff / length;
    float n = range[0];
    for (int i = 0; i < length; i++)
    {
        list[i] = n;
        n += increase;
    }
    return list;
}