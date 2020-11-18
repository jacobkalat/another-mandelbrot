//
// Created by jacob on 9/10/20.
//
#include "graphicslibrary.h"
#include "mandelbrot.h"

unsigned int iterations(double complex p0,unsigned int max_iteration);

rgb_image_t * calculate_mandelbrot(double complex min,
                                    double complex max,
                                    unsigned int steps_real,
                                    unsigned int steps_imaginary,
                                    unsigned int max_iteration)
{

    const double delta_real = creal(max-min)/(1.0*steps_real);
    const double delta_imaginary = cimag(max-min)/(1.0*steps_imaginary);
    const double start_real = creal(min);
    const double start_imaginary = cimag(min);
    const unsigned int background=2;

    rgb_image_t * image;
    image = malloc(sizeof(rgb_image_t));
    image->image_size_x=steps_real;
    image->image_size_y=steps_imaginary;
    image->image_data = malloc(image->image_size_x * image->image_size_y * 3);

    double im = start_imaginary;
    int image_ptr=0;

    for (int c=0;c<steps_imaginary;c++) {
        double re = start_real;

        for (int r = 0; r < steps_real; r++) {
            int count = iterations(re+im*1i, max_iteration);
            rgb_t p;
            if(count>background)
                hsv_to_rgb(count, background, max_iteration, &p);
            else{p.r=0;p.g=0;p.b=0;}
            image->image_data[image_ptr++]=p.r;
            image->image_data[image_ptr++]=p.g;
            image->image_data[image_ptr++]=p.b;

            re += delta_real;
        }
        im += delta_imaginary;

    }
    return image;
}


unsigned int iterations(double complex p0,unsigned int max_iteration)
{
    unsigned int icount = 0;
    double complex p = p0;

    //Where the mandelbrot calculations actually happens
    while (cabs(p)<2 && icount<max_iteration)
    {
        p = p*p+p0;
        icount++;
    }

    return icount;
}