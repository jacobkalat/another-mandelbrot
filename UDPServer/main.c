
//https://www.geeksforgeeks.org/udp-server-client-implementation-c/
// Server side implementation of UDP client-server model

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <pthread.h>
#include "UDP_Routines.h"

#include <libconfig.h>

#define PORT     8080

int gsockfd;
double mandelbrot_scale = 1.0;
double mandelbrot_imaginary_center = 0.0;
double mandelbrot_real_center = 0.0;


// Driver code
int main() {

    static int count=0;
    int uxds;
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
    config_lookup_int(cf,"defaults.uxds",&uxds);

    if(uxds == 0)
    {
        gsockfd=open_inet_udp_socket(PORT);
    }
    else
    {
        gsockfd=open_uxds_udp_socket();
    }

    pthread_t tid;
    while (1) {
        //  await request

        struct rqst_udp_pkt *rqst=make_rqst(uxds);
        rqst->number=count;
        await_request(rqst,uxds);
        pthread_create(&tid, NULL, send_response, (void *) rqst);
        count++;
    }
    pthread_join(tid,NULL);  // should wait for all threads to terminate.
    return 0;
}