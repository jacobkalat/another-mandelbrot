//
// Created by jacob on 11/10/2020
//
#include <complex.h>
#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H
rgb_image_t * calculate_mandelbrot(double complex min,
                                    double complex max,
                                    unsigned int steps_real,
                                    unsigned int steps_imaginary,
                                    unsigned int max_iteration);
#endif //MANDELBROT_MANDELBROT_H
