#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "usage_check_functions.h"


int usage_conrtol(int argc, char *argv[], usagereturn *ret)
{
    if (argc < 2 || argc > 6)
    {
        return 1;
    }
    float xnum = -0.6;
    float ynum = 0;
    float rangenum = 4.0;
    int maxloop = 100;
    int xres = 1920;
    int yres = 1080;

    (*ret).px = xnum;
    (*ret).py = ynum;
    (*ret).range = rangenum;
    (*ret).looplengh = maxloop;
    (*ret).xres = xres;
    (*ret).yres = yres;

    for (int i = 1; i < argc; i++)
    {
        int check;
        check = fractal(argv, i, ret);
        if (check != 0)
        {
            return check;
        }

        check = center(argv, i, ret);
        if (check != 0)
        {
            return check;
        }
        
        check = range(argv, i, ret);
        if (check != 0)
        {
            return check;
        }

        check = looplength(argv, i, ret);
        if (check != 0)
        {
            return check;
        }
        
        check = resolution(argv, i, ret);
        if (check != 0)
        {
            return check;
        }
    }
    return 0;
}

int fractal(char *argv[], int i, usagereturn *ret)
{
    if (i != 1)
    {
        return 0;
    }
    int fractanum;
    if (strstr("MmBb", argv[i]) == NULL || strlen(argv[i]) != 1)
    {
        return 1;
    }
    fractanum = toupper(argv[i][0]);
    (*ret).fractal = fractanum;
    return 0;
}

int center(char *argv[], int i, usagereturn *ret)
{
    if (i != 2)
    {
        return 0;
    }
    float xnum = -0.6;
    float ynum = 0;
    char *center = argv[i];
    int len = strlen(center);
    int index = 0;
    if (len < 3 || (!isdigit(center[index]) && strchr("+-", center[index]) == NULL))
    {
        printf("k\n");
        return 2;
    }
    char *x = malloc(len);
    int xindex = 0;
    while (center[index] != ',')
    {
        if (!isdigit(center[index]) && strchr("+-.", center[index]) == NULL)
        {
            free(x);
            return 2;
        }
        x[xindex] = center[index];
        xindex += 1;
        index += 1;
    }
    index += 1;
    x[xindex] = '\0';
    xnum = strtof(x, NULL);
    free(x);

    char *y = malloc(len);
    int yindex = 0;
    while (index < len)
    {
        if (!isdigit(center[index]) && strchr("+-.", center[index]) == NULL)
        {
            free(y);
            return 2;
        }
        y[yindex] = center[index];
        yindex += 1;
        index += 1;
    }
    y[yindex] = '\0';
    ynum = strtof(y, NULL);
    free(y);

    (*ret).px = xnum;
    (*ret).py = ynum;
    return 0;
}

int range(char *argv[], int i, usagereturn *ret)
{
    // range limit is 0.00005
    float rangenum = 4.0;
    if (i != 3)
    {
        return 0;
    }
    char *range = argv[i];
    rangenum = strtof(range, NULL);
    if (rangenum <= 0)
    {
        return 3;
    }
    (*ret).range = rangenum;
    return 0;
}

int looplength(char *argv[], int i, usagereturn *ret)
{
    int maxloop = 100;
    if (i != 4)
    {
        return 0;
    }
    maxloop = atoi(argv[i]);
    if (maxloop <= 0)
    {
        return 4;
    }
    (*ret).looplengh = maxloop;
    return 0;
}

int resolution(char *argv[], int i, usagereturn *ret)
{
    int xres = 1920;
    int yres = 1080;
    if (i != 5)
    {
        return 0;
    }
    xres = 0;
    yres = 0;
    if (!strcmp(argv[i], "UHD"))
    {
        xres = 3840;
        yres = 2160;
    }
    if (!strcmp(argv[i], "HD"))
    {
        xres = 1920;
        yres = 1080;
    }
    if (!strcmp(argv[i], "ED"))
    {
        xres = 1280;
        yres = 720;
    }
    if (!strcmp(argv[i], "SD"))
    {
        xres = 720;
        yres = 480;
    }
    if (xres == 0)
    {
        return 5;
    }
    (*ret).xres = xres;
    (*ret).yres = yres;
    return 0;
}

void usage_print(int check)
{
    printf("usage ./fracatl_image_craetion FRACTAL ..options\n");
    if (check == 1)
    {
        printf("M for the mandel brot fractal\
        \nB for the burnig ship fractal\n");
        return;
    }
    printf("options: CENTER RANGE FRACTAL_LOOP_LENGTH RESOLUTION\n");
    if (check == 2)
    {
        printf("option CENTER: num,num\n");
        return;
    }
    if (check == 3)
    {
        printf("option RANGE: pos num\n");
        return;
    }
    if (check == 4)
    {
        printf("option MAX_LOOP_LENGTH: pos num\n");
        return;
    }
    if (check == 5)
    {
        printf("option RESOLUTION:\nUHD for 4k\nHD for 1080p\
        \nED for 720p\nSD for 480p\n");
        return;
    }
}