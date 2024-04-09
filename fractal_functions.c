#include <stdio.h>
#include <math.h>
#include "fractal_functions.h"

BYTE mandelbrot(float x, float iy, int maxlength)
{    
    float ic[] = {x, iy};
    float z[] = {x, iy};
    for (int i = 1; i < maxlength; i++)
    {
        if (pow((z[0]*z[0] + z[1]*z[1]), 0.5) > mandelbrot_EscapeValue)
        {
            return i;
        }
        float temp = z[0]*z[0] - z[1]*z[1] + ic[0];
        z[1] = 2 * z[0] * z[1] + ic[1];
        z[0] = temp;
    }
    return 0;
}

BYTE burningship(float x, float iy, int maxlength)
{
    float ic[] = {x, iy};
    float z[] = {x, iy};
    for (int i = 1; i < maxlength; i++)
    {
        if (pow((z[0]*z[0] + z[1]*z[1]), 0.5) > burningship_EscapeValue)
        {
            return i;
        }
        z[0] = fabsf(z[0]);
        z[1] = fabsf(z[1]);
        float temp = z[0]*z[0] - z[1]*z[1] + ic[0];
        z[1] = 2 * z[0] * z[1] + ic[1];
        z[0] = temp;
    }
    return 0;    
}
