#include <stdio.h>
#include <libconfig.h>
#include <argp.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct arguments
{
    double realCenter;
    double imagCenter;
    int realSegments;
    int imagSegments;
    double scale;
    int image_size;
    char fileN[20];
    int uxds;
};

static int parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'r':
            arguments->realCenter = atof(arg);
            break;
        case 'i':
            arguments->imagCenter = atof(arg);
            break;
        case 's':
            arguments->scale = atof(arg);
            break;
        case 'x':
            arguments->realCenter = atof(arg);
            break;
        case 'y':
            arguments->imagCenter = atof(arg);
            break;
        case 'f':
            strcpy(arguments->fileN, arg);
            break;
    }
    return 0;
}

int main(int argc, char **argv) {
    struct arguments mandyArgs;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Set up the config file
    config_t cfg, *cf;
    cf = &cfg;
    config_init(cf);

    if (!config_read_file(cf, "/home/jake/.config/mandelbrot/mandy.cfg")) {
        fprintf(stderr, "%s:%d - %s\n",
                config_error_file(cf),
                config_error_line(cf),
                config_error_text(cf));
        config_destroy(cf);
        return(EXIT_FAILURE);
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Set some values from the config file
    config_lookup_float(cf,"defaults.real_center", &mandyArgs.realCenter);
    config_lookup_int(cf,"defaults.real_segments", &mandyArgs.realSegments);

    config_lookup_float(cf,"defaults.imaginary_center", &mandyArgs.imagCenter);
    config_lookup_int(cf,"defaults.imaginary_segments", &mandyArgs.imagSegments);

    config_lookup_int(cf,"defaults.n", &mandyArgs.image_size);
    config_lookup_float(cf,"defaults.scale", &mandyArgs.scale);

    config_lookup_int(cf,"defaults.uxds",&mandyArgs.uxds);

    const char *filename;
    config_lookup_string(cf,"defaults.filename", &filename);
    strcpy(mandyArgs.fileN,filename);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Setup command line parsing and parse (override config file)
    struct argp_option options[] = {
            {"realCenter",  'r', "NUM",       0, "real center" },
            {"imagCenter",  'i', "NUM",       0, "imaginary center" },
            {"realCenter",  'x', "NUM",       0, "user input real center" },
            {"imagCenter",  'y', "NUM",       0, "user input imaginary center" },
            {"scale",      's', "NUM",       0, "scale"},
            {"filename",       'f', "",       0, "Filename for the ppm"},
            { 0 }
    };
    //Parse args
    struct argp argp = { options, parse_opt, 0,0};
    argp_parse (&argp, argc, argv, 0, 0,&mandyArgs);

    printf("Real Center: %f\n"
           "Real Segments: %d\n"
           "Imaginary Center: %f\n"
           "Imaginary Segments: %d\n"
           "Image Size: %d\n"
           "Scale: %f\n"
           "UXDS: %d\n"
           "Filename: %s\n",
           mandyArgs.realCenter,mandyArgs.realSegments,mandyArgs.imagCenter,mandyArgs.imagSegments,mandyArgs.image_size,mandyArgs.scale,mandyArgs.uxds,mandyArgs.fileN);
}
