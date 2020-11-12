// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <libconfig.h>
#include <argp.h>
#include <complex.h>
#include "graphicslibrary.h"

#define PORT	 8080
#define MAXLINE 1024

double real_center;
int real_segments;

double imaginary_center;
int imaginary_segments;

int n;
int scale;

const char* filename;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Request packet struct
struct rqst_udp_pkt{
    int number;
    struct sockaddr_in * inet_cliaddr;
    struct sockaddr_in * inet_svraddr;
    int len;
    char *rqst_data;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Request packet setup
struct rqst_udp_pkt * make_rqst()
{
    struct rqst_udp_pkt *rqst = (struct rqst_udp_pkt *) malloc(sizeof(struct rqst_udp_pkt));
    rqst -> inet_cliaddr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    rqst -> inet_svraddr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    rqst -> rqst_data = (char *) malloc(MAXLINE);
    return rqst;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Open network socket
int open_inet_server_socket(int port, struct rqst_udp_pkt * rqst_pkt)
{
    int sockfd;
    struct sockaddr_in	 servaddr;
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(rqst_pkt->inet_svraddr, 0, sizeof(struct sockaddr_in));

    // Filling server information
    rqst_pkt->inet_svraddr->sin_family = AF_INET;
    rqst_pkt->inet_svraddr->sin_port = htons(PORT);
    rqst_pkt->inet_svraddr->sin_addr.s_addr = INADDR_ANY;

    return sockfd;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Send requests
void send_requests(int sockfd,const struct sockaddr * svraddr,
                   int image_number,
                   complex double center,double scale,int real_segs,int imaginary_segs,int n)
{
    const double extent = 3.0;
    int n_real = n/real_segs;           // n needs to be multiple of real_segs
    int n_imaginary = n/imaginary_segs; // n needs to be multiple of imaginary_segs
    char buffer[128];
    const double delta_real = extent/(1.0*n_real*scale);
    const double delta_imaginary = extent/(1.0*n_imaginary*scale);
    const double start_real = creal(center)-extent/(2.*scale);
    const double start_imaginary = cimag(center)-extent/(2.*scale);

    // we  need to send start_real,start_imaginary,end_real,end_imaginary,n_real,n_imaginary)
    int r_counter,r_start;
    int i_counter,i_start;

    double real_offset,real_end,imaginary_offset,imaginary_end;
    for (r_counter=0,r_start=0;r_counter<real_segs; r_counter++,r_start+=n_real)
        for (i_counter=0,i_start=0;i_counter<imaginary_segs; i_counter++,i_start+=n_imaginary){
            // make a packet
            real_offset  = start_real + r_counter*delta_real;
            real_end  = real_offset + delta_real;

            imaginary_offset  = start_imaginary + i_counter*delta_imaginary;
            imaginary_end  = imaginary_offset + delta_imaginary;
            printf("\n %lf,%lf,%d,%lf,%lf,%d",real_offset,real_end,n_real,imaginary_offset,imaginary_end,n_imaginary);
            sprintf(buffer,"%d %d %d %lf,%lf,%d,%lf,%lf,%d",image_number,r_start,i_start,real_offset,real_end,n_real,imaginary_offset,imaginary_end,n_imaginary);
            int len = sizeof( struct sockaddr_in);
            sendto(sockfd, (const char *)buffer, strlen(buffer),
                   MSG_CONFIRM, (const struct sockaddr *) svraddr,
                   len);



        }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Receive response from server
void await_responses(int sockfd,struct rqst_udp_pkt * rqst_pkt,int expected_number,int image_size){
    // expected image number
    // dump all packets which are not expected
    // we know how many packets for a completed image wait for those
    // if time_out request resend... this should not happen on a single machine

    int image_number, n;
    unsigned char buffer[MAXLINE];
    int len = sizeof(struct sockaddr_in);

    rgb_image_t *image;
    image = malloc(sizeof(rgb_image_t));
    image->image_size_x=image_size;
    image->image_size_y=image_size;
    image->image_data = malloc(image->image_size_x * image->image_size_y * 3);


    while (expected_number-- > 0) {
        n = recvfrom(sockfd, (char *) buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *) rqst_pkt->inet_svraddr,
                     &len);
        printf("\n Data-->");
        printf("%d,%d,%d,%d %d %d:",
               ((int *)buffer)[0],((int *)buffer)[1],
               ((int *)buffer)[2],((int *)buffer)[3],
               ((int *)buffer)[4],((int *)buffer)[5]
        );
        for(int i=0;i<n;i++)
            printf("%d,",buffer[i+6*sizeof(int)]);
    }

    // this could return the populated rgb image..
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Parsing function
static int
parse_opt(int key,char *arg, struct argp_state *state)
{
    switch(key)
    {
        case 'x':
        {
            real_center = atof(arg);
            break;
        }
        case 'e':
        {
            real_segments = atoi(arg);
            break;
        }
        case 'y':
        {
            imaginary_center = atof(arg);
            break;
        }
        case 'm':
        {
            imaginary_segments = atoi(arg);
            break;
        }
        case 's':
        {
            scale = atoi(arg);
            break;
        }
        case 'n':
        {
            n = atoi(arg);
            break;
        }
        case 'f':
        {
            strcpy(filename, arg);
            break;
        }
    }
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Driver code
int main(int argc, char **argv)
{
    //Set up receive buffer for incoming packets from server
    char buffer[MAXLINE];

    //Data format "x y scale n max_iterations"
    char *hello = "0 0 2 512 1024";

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Setting up the socket
    int sockfd;
    struct rqst_udp_pkt * rqst_pkt= make_rqst();
    sockfd = open_inet_server_socket(PORT, rqst_pkt);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Set up the config file
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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Set some values from the config file
    config_lookup_float(cf,"defaults.real_center", &real_center);
    config_lookup_int(cf,"defaults.real_segments", &real_segments);

    config_lookup_float(cf,"defaults.imaginary_center", &imaginary_center);
    config_lookup_int(cf,"defaults.imaginary_segments", &imaginary_segments);

    config_lookup_int(cf,"defaults.n", &n);
    config_lookup_int(cf,"defaults.scale", &scale);
    config_lookup_string(cf,"defaults.filename", &filename);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Parse command line args

    struct argp_option options[] =
            {
                    {"real_center",'x',"NUM",0,"Mandelbrot real center"},
                    {"real_segments",'e',"NUM",0,"Mandelbrot real segments"},
                    {"imaginary_center",'y',"NUM",0,"Mandelbrot imaginary center"},
                    {"imaginary_segments",'m',"NUM",0,"Mandelbrot imaginary segments"},
                    {"scale",'s',"NUM",0,"Mandelbrot scale"},
                    {"n",'n',"NUM",0,"Mandelbrot n"},
                    {"filename",'f',0,0,"Name output ppm file"},
                    { 0 }
            };
    struct argp argp = { options, parse_opt,0,0};
    argp_parse(&argp,argc,argv,0,0,0);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Test some stuff
    printf("%f %d %f %d %d %d %s\n",real_center, real_segments, imaginary_center, imaginary_segments, scale, n, filename);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Send the requests
    complex double center = real_center+imaginary_center*I;
    int image_size = imaginary_segments*real_segments;

    send_requests(sockfd,(struct sockaddr *) rqst_pkt->inet_svraddr,10,center,scale,real_segments,imaginary_segments,n);
    await_responses(sockfd, rqst_pkt,n,image_size);

    close(sockfd);
    return 0;
}