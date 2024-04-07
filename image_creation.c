// this program outputs a bmp image for a shosen fractal
// centerd at a given cordenate in given range

// takes fractal type image daimentions center point range length  escape value and loop length

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "fractal_functions.c"

float *centeredrangelist(float list_center, float range, int size);
float *lengthrangelist(float range[2], int length);
void wrongusagehandeler(int check);
typedef struct
{
    char fractal;
    float px;
    float py;
    float range;
    int looplengh;
    int xres;
    int yres;
} usagereturn;
int usage_conrtol(int argc, char *argv[], usagereturn *ret);
int fractal(char *argv[], int i, usagereturn *ret);
int center(char *argv[], int i, usagereturn *ret);
int range(char *argv[], int i, usagereturn *ret);
int looplength(char *argv[], int i, usagereturn *ret);
int resolution(char *argv[], int i, usagereturn *ret);

const int zero = 0;
const int zero_array[] = {0, 0, 0, 0};
const int one = 1;
const int type = 0x4d42; // ascii hex for "BM"
const int BI_RGB = 0x00;
const int colotpalet = 24; // bit

int main(int argc, char *argv[])
{
    usagereturn user_usage;
    int check = usage_conrtol(argc, argv, &user_usage);
    if (check)
    {
        wrongusagehandeler(check);
        return 1;
    }
    printf("this is the good good fractal creation software made by ghamdi lmt\n");

    int maxlooplength = user_usage.looplengh;
    int hight = user_usage.yres;
    int width = user_usage.xres;
    float xcenter = user_usage.px;  // mandelbrot -0.7 burningship -0.5
    float ycenter = user_usage.py;  // mandelbrot 0 burningship 0.6
    float range = user_usage.range; // mandelbrot 2.4 burningship 3
    // Declaration of function pointer variable (thanks gpt)
    BYTE(*Fractal)
    (float x, float iy, int maxlength);
    if (user_usage.fractal == 'M')
    {
        Fractal = mandelbrot;
    }
    if (user_usage.fractal == 'B')
    {
        Fractal = burningship;
    }

    float xrange = range * ((float)width / hight);
    float yrange = range;
    float *x = centeredrangelist(xcenter, xrange, width);
    float *y = centeredrangelist(ycenter, yrange, hight);
    if (x == NULL || y == NULL)
    {
        return 2;
    }

    FILE *output = fopen("fractal.bmp", "wb");
    if (output == NULL)
    {
        return 2;
    }

    int Bytes_Per_Raster = width * 3;
    int Raster_padding = (4 - (Bytes_Per_Raster % 4)) % 4;
    Bytes_Per_Raster += Raster_padding;

    BITMAPFILEHEADER BFH;
    BFH.bfType = type;
    BFH.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + Bytes_Per_Raster * width;
    BFH.bfReserved1 = zero;
    BFH.bfReserved2 = zero;
    BFH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER BIH;
    BIH.biSize = sizeof(BITMAPINFOHEADER);
    BIH.biWidth = width;
    BIH.biHeight = hight;
    BIH.biPlanes = one;
    BIH.biBitCount = colotpalet;
    BIH.biCompression = BI_RGB;
    BIH.biSizeImage = Bytes_Per_Raster * hight; // zero for uncommpresed image
    BIH.biXPelsPerMeter = zero;                 // zero for unspecified resolution
    BIH.biYPelsPerMeter = zero;                 // zero for unspecified resolution
    BIH.biClrUsed = zero;                       // zero for no Palette in 24 bit mode
    BIH.biClrImportant = zero;                  // zero for all colors are important or no Palette in 24 bit mode

    fwrite(&BFH, sizeof(BFH), 1, output);
    fwrite(&BIH, sizeof(BIH), 1, output);

    RGBTRIPLE(*img_array)
    [width] = calloc(hight, width * sizeof(RGBTRIPLE));
    BYTE(*escape_array)
    [width] = calloc(hight, width * sizeof(BYTE));
    int max_escape_val = 0;
    if (img_array == NULL || escape_array == NULL)
    {
        fclose(output);
        return 3;
    }

    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            escape_array[i][j] = Fractal(x[j], -y[i], maxlooplength);
            if (escape_array[i][j] > max_escape_val)
            {
                max_escape_val = escape_array[i][j];
            }
        }
    }

    float color_range[] = {0, 255};
    float *color_array = lengthrangelist(color_range, max_escape_val + 1);

    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int color = round(color_array[escape_array[i][j]]);
            img_array[i][j].rgbtRed = color;
            img_array[i][j].rgbtGreen = color;
            img_array[i][j].rgbtBlue = color;
        }
    }

    for (int i = 0; i < hight; i++)
    {
        fwrite(img_array[i], sizeof(*img_array), 1, output);
        for (int j = 0; j < Raster_padding; j++)
        {
            fwrite(zero_array, sizeof(BYTE), 1, output);
        }
    }

    free(y);
    free(x);
    free(color_array);
    free(escape_array);
    free(img_array);
    fclose(output);
    return 0;
}

float *centeredrangelist(float list_center, float range, int size)
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

float *lengthrangelist(float range[2], int length)
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

void wrongusagehandeler(int check)
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