//
// Created by joel on 9/2/20.
//


#ifndef SIMPLEGRAPHICS_GRAPHICS_UTILITIES_H
#define SIMPLEGRAPHICS_GRAPHICS_UTILITIES_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rgb{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_t;

typedef struct pgm{
    unsigned char gray;
} pgm_t;

typedef struct rgb_image {
    unsigned int image_size_x;
    unsigned int image_size_y;
    unsigned char *image_data;
} rgb_image_t;

typedef struct pgm_image {
    unsigned int image_size_x;
    unsigned int image_size_y;
    unsigned char *image_data;
} pgm_image_t;

void hsv_to_rgb(int hue, int min, int max, rgb_t *p);

void write_rgb_file(char *filename,rgb_image_t *image);
void write_rgb_pipe(rgb_image_t *image);
void write_pgm_file(char *filename, pgm_image_t *image);
void write_pgm_pipe(pgm_image_t *image);
rgb_image_t * read_ppm_rgb_file(char *file);
rgb_image_t * read_ppm_rgb_pipe();
rgb_image_t *read_ppm_rgb_mandy();

void free_rgb_image(rgb_image_t *image);
rgb_image_t * get_ppm(FILE *pf);
#endif //SIMPLEGRAPHICS_GRAPHICS_UTILITIES_H
