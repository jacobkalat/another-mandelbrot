//
// Created by joel on 9/2/20.
//

#include "graphics_utilities.h"
#include "math.h"
// Adapted from http://rosettacode.org/wiki/Mandelbrot_set#C


#define Num 255
int color_rotate = 0;
int saturation = 1;
int invert = 0;

float set_saturation(float new_sat)
{
    saturation = new_sat;
}



void hsv_to_rgb(int hue, int min, int max, rgb_t *p)
{
    //Deal with certain conditions
    if (min == max) max = min + 1;
    if (invert) hue = max - (hue - min);
    if (!saturation) {
        p->r = p->g = p->b = 255 * (max - hue) / (max - min);
        return;
    }
    //Set up RGB conversion here
    double h = fmod(color_rotate + 1e-4 + 4.0 * (hue - min) / (max - min), 6);
#	define VAL 255
    double c = VAL * saturation;
    double X = c * (1 - fabs(fmod(h, 2) - 1));

    //Reset r,g,and b values before setting the new ones
    p->r = p->g = p->b = 0;

    //Put RGB value into P
    switch((int)h) {
        case 0: p->r = c; p->g = X; return;
        case 1:	p->r = X; p->g = c; return;
        case 2: p->g = c; p->b = X; return;
        case 3: p->g = X; p->b = c; return;
        case 4: p->r = X; p->b = c; return;
        default:p->r = c; p->b = X;
    }
}

rgb_image_t *read_ppm_rgb_file(char *filename){
    FILE * fp = fopen( filename, "r");
    if (fp == NULL) return NULL;
    rgb_image_t *image;
    image = get_ppm(fp);
    return image;
}

void write_rgb_file(char *filename,rgb_image_t *image) {

    int i, j,im_ptr;
    //Open stream to ppm file
    FILE *fp = fopen(filename, "wb"); /* b - binary mode */

    //Set up header for PPM file, Magic number, size, color limitations
    fprintf(fp, "P6\n%d %d\n255\n", image->image_size_x, image->image_size_y);

    //Initialize image pointer
    im_ptr =0;

    //Write RGB data to the PPM file opened above
    for (j = 0; j < image->image_size_y; ++j) {
        for (i = 0; i < image->image_size_y; ++i) {
            static unsigned char color[3];
            color[0] = image->image_data[im_ptr++];  /* red */
            color[1] = image->image_data[im_ptr++];  /* green */
            color[2] = image->image_data[im_ptr++];  /* blue */
            fwrite(color, 1, 3, fp);
        }
    }
    //Close stream to ppm file when done writing
    fclose(fp);
}

void write_rgb_pipe(rgb_image_t *image)
{
    int i, j,im_ptr;

    //Open stream to standard output
    FILE *fp = stdout;/* b - binary mode */
    //Set up header, magic number, size, and color limitations
    fprintf(fp, "P6\n%d %d\n255\n", image->image_size_x, image->image_size_y);
    im_ptr =0;
    for (j = 0; j < image->image_size_y; ++j) {
        for (i = 0; i < image->image_size_y; ++i) {
            static unsigned char color[3];
            color[0] = image->image_data[im_ptr++];  /* red */
            color[1] = image->image_data[im_ptr++];  /* green */
            color[2] = image->image_data[im_ptr++];  /* blue */
            fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);
}


rgb_image_t *read_ppm_rgb_pipe(){

    rgb_image_t *image;
    image = get_ppm(stdin);


    return image;

}
rgb_image_t *read_ppm_rgb_mandy(){

    rgb_image_t *image;


    FILE *fp = popen("cat Mandy.ppm", "r");
    image = get_ppm(fp);

    pclose(fp);

    return image;

}

#define PPMREADBUFLEN 1024
rgb_image_t * get_ppm(FILE *pf)
{
    // adapted from https://rosettacode.org/wiki/Bitmap/Read_a_PPM_file#C
    // i
    unsigned char buf[PPMREADBUFLEN], *t;
    rgb_image_t *image;


    unsigned int w, h, d;
    int r;

    if (pf == NULL) return NULL;
    t = fgets(buf, PPMREADBUFLEN, pf);
    /* the code fails if the white space following "P6" is not '\n' */
    if ( (t == NULL) || ( strncmp(buf, "P6\n", 3) != 0 ) ) return NULL;
    do
    { /* Px formats can have # comments after first line */
        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( t == NULL ) return NULL;
    } while ( strncmp(buf, "#", 1) == 0 );
    r = sscanf(buf, "%u %u", &w, &h);
    if ( r < 2 ) return NULL;

    r = fscanf(pf, "%u", &d);
    if ( (r < 1) || ( d != 255 ) ) return NULL;
    // fseek(pf, 1, SEEK_CUR); /* skip one byte, should be whitespace, doesn't seem to work on pipes */
    t = fgets(buf, 1, pf);

    image = malloc(sizeof(rgb_image_t));
    image->image_size_x=w;
    image->image_size_y=h;
    image->image_data = malloc(image->image_size_x * image->image_size_y * 3);
    if ( image->image_data != NULL )
    {
        size_t rd = fread(image->image_data, sizeof(char), 3*w*h, pf);
        if ( rd < 3*w*h )
        {
            free_rgb_image(image);
            return NULL;
        }
        return image;
    }
}


void free_rgb_image(rgb_image_t *image)
{
    free (image->image_data);
    free (image);
}
