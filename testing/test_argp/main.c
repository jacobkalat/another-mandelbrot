#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <argz.h>

static int
parse_opt(int key,char *arg, struct argp_state *state)
{
    switch(key)
    {
        case 'x':
        {
            printf("\nx option was selected\n");
            break;
        }
        case 'y':
        {
            printf("\ny option was selected\n");
            break;
        }
        case 's':
        {
            printf("\ns option was selected\n");
            break;
        }
        case 'f':
        {
            printf("\nf option was selected\n");
            break;
        }
    }
    return 0;
}

int
main (int argc, char **argv)
{
    struct argp_option options[] =
            {
                    {"real_center",'x',0,0,"Mandelbrot real center"},
                    {"imaginary_center",'y',0,0,"Mandelbrot imaginary center"},
                    {"scale",'s',0,0,"Mandelbrot scale"},
                    {"filename",'f',0,0,"Name output ppm file"},
                    { 0 }
            };
    int arg_count = 4;
    struct argp argp = { options, parse_opt,0,0};
    argp_parse(&argp,argc,argv,0,0,0);
    return 0;
}