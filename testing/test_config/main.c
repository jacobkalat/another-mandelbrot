#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>


// Demo From https://jpmens.net/2009/09/17/reading-configuration-files-with-libconfig/

int main(int argc, char **argv)
{
    config_t cfg, *cf;
    cf = &cfg;
    config_init(cf);

    if (!config_read_file(cf, "mandy.cfg")) {
        fprintf(stderr, "%s:%d - %s\n",
                config_error_file(cf),
                config_error_line(cf),
                config_error_text(cf));
        config_destroy(cf);
        return(EXIT_FAILURE);
    }

    //Lookup a float
    double center;
    config_lookup_float(cf, "defaults.center", &center);
    printf("Center: %f\n", center);

    //Lookup a string
    char *filename = "";
    config_lookup_string(cf, "defaults.filename",(const char **) &filename);
    printf("Filename: %s\n", filename);

    //Lookup int
    int uxds;
    config_lookup_int(cf, "defaults.uxds",&uxds);
    printf("UXDS: %d\n",uxds);

    config_destroy(cf);
    return 0;
}
