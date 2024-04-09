// this program outputs a bmp image for a shosen fractal
// centerd at a given cordenate in given range

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fractal_functions.c"
#include "range_list_creators.c"
#include "usage_check_functions.c"

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
        usage_print(check);
        return 1;
    }
    printf("this is the good good fractal creation software made by ghamdi lmt\n");

    int maxlooplength = user_usage.looplengh;
    int hight = user_usage.yres;
    int width = user_usage.xres;
    float xcenter = user_usage.px;  
    float ycenter = user_usage.py;  
    float range = user_usage.range;

    // Declaration of function pointer variable (thanks gpt)
    BYTE(*fractal_function)(float x, float iy, int maxlength);
    if (user_usage.fractal == 'M')
    {
        fractal_function = mandelbrot;
    }
    if (user_usage.fractal == 'B')
    {
        fractal_function = burningship;
    }

    float xset_range = range * ((float)width / hight);
    float yset_range = range;
    float *x = centered_rangelist(xcenter, xset_range, width);
    float *y = centered_rangelist(ycenter, yset_range, hight);
    if (x == NULL || y == NULL)
    {
        return 2;
    }

    int Bytes_Per_Raster = width * 3;
    int Raster_padding = (4 - (Bytes_Per_Raster % 4)) % 4;
    Bytes_Per_Raster += Raster_padding;

    FILE *output = fopen("fractal.bmp", "wb");
    if (output == NULL)
    {
        return 2;
    }

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
    BIH.biSizeImage = Bytes_Per_Raster * hight; 
    BIH.biXPelsPerMeter = zero;
    BIH.biYPelsPerMeter = zero;
    BIH.biClrUsed = zero;
    BIH.biClrImportant = zero;

    fwrite(&BFH, sizeof(BFH), 1, output);
    fwrite(&BIH, sizeof(BIH), 1, output);

    RGBTRIPLE(*img_array)
    [width] = calloc(hight, width * sizeof(RGBTRIPLE));

    BYTE(*escape_array)
    [width] = calloc(hight, width * sizeof(BYTE));

    if (img_array == NULL || escape_array == NULL)
    {
        fclose(output);
        return 3;
    }

    int max_escape_val = 0;
    for (int i = 0; i < hight; i++)
    {
        for (int j = 0; j < width; j++)
        {
            escape_array[i][j] = fractal_function(x[j], -y[i], maxlooplength);
            if (escape_array[i][j] > max_escape_val)
            {
                max_escape_val = escape_array[i][j];
            }
        }
    }

    float color_range[] = {0, 255};
    float *color_array = length_rangelist(color_range, max_escape_val + 1);

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

