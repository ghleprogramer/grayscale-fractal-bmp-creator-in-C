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
void usage_print(int check);
