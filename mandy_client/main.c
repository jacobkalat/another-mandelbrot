// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <libconfig.h>
#include <argp.h>
#include <complex.h>
#include "graphicslibrary.h"

#define MAXLINE 1024
#define HEADER_SZ (6*sizeof(int))
#define MANDY_MAX (HEADER_SZ + (512*512*3))

double real_center = 0.0;
int real_segments = 16;
double imaginary_center = 0.0;
int imaginary_segments = 16;
int image_size = 1024;
double scale = 2.0;
char filename[20];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Request packet struct
struct rqst_udp_pkt{
    struct sockaddr_un * uxds_cliaddr;
    struct sockaddr_un * uxds_svraddr;
    int len;
    char *rqst_data;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Request packet setup
struct rqst_udp_pkt * make_rqst()
{
    struct rqst_udp_pkt *rqst = (struct rqst_udp_pkt *) malloc(sizeof(struct rqst_udp_pkt));
    rqst -> uxds_cliaddr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
    rqst -> uxds_svraddr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
    rqst -> rqst_data = (char *) malloc(MAXLINE);
    return rqst;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Open unix domain socket
int open_uxds_server_socket(char * uxds_udp_server_path, struct rqst_udp_pkt * rqst_pkt)
{
    int socket_fd;

    if((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {

        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset((* rqst_pkt).uxds_cliaddr, 0, sizeof(struct sockaddr_un));
    (* (* rqst_pkt).uxds_cliaddr).sun_family = AF_UNIX;
    strcpy((* (* rqst_pkt).uxds_cliaddr).sun_path, "/tmp/UDSDGCLNT");  // This might be best to pass in

    unlink((* (* rqst_pkt).uxds_cliaddr).sun_path);
    if(bind(socket_fd, (const struct sockaddr *) (* rqst_pkt).uxds_cliaddr,
            sizeof(struct sockaddr_un)) < 0)
    {
        perror("client: bind");
        return 1;
    }

    memset(rqst_pkt->uxds_svraddr, 0, sizeof(struct sockaddr_un));
    rqst_pkt->uxds_svraddr->sun_family = AF_UNIX;
    strcpy(rqst_pkt->uxds_svraddr->sun_path, uxds_udp_server_path);

    return socket_fd;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Send requests
void send_requests(int sockfd,const struct sockaddr * svraddr,
                   int image_number,
                   complex double center,double req_scale,int real_segs,int imaginary_segs,int n)
{
    const double extent = 3.0;
    int n_real = n/real_segs;           // n needs to be multiple of real_segs
    int n_imaginary = n/imaginary_segs; // n needs to be multiple of imaginary_segs
    char buffer[128];
    const double delta_real = extent/(1.0*real_segs*req_scale);
    const double delta_imaginary = extent/(1.0*imaginary_segs*req_scale);
    const double start_real = creal(center)-extent/(2.*req_scale);
    const double start_imaginary = cimag(center)-extent/(2.*req_scale);

    // we  need to send start_real,start_imaginary,end_real,end_imaginary,n_real,n_imaginary)
    int r_counter,r_start;
    int i_counter,i_start;

    double real_offset,real_end,imaginary_offset,imaginary_end;
    for (r_counter=0,r_start=0;r_counter<real_segs; r_counter++,r_start+=n_real) {
        for (i_counter = 0, i_start = 0; i_counter < imaginary_segs; i_counter++, i_start += n_imaginary) {
            // make a packet
            real_offset = start_real + r_counter * delta_real;
            real_end = real_offset + delta_real;

            imaginary_offset = start_imaginary + i_counter * delta_imaginary;
            imaginary_end = imaginary_offset + delta_imaginary;
//            printf("\n %lf,%lf,%lf,%d,%lf,%lf,%d",
//                   real_offset, real_end, delta_real,n_real,
//                   imaginary_offset, imaginary_end,n_imaginary);
            sprintf(buffer, "%d %d %d %lf,%lf,%d,%lf,%lf,%d", image_number, r_start, i_start, real_offset, real_end,
                    n_real, imaginary_offset, imaginary_end, n_imaginary);
            int len = sizeof(struct sockaddr_un);
            sendto(sockfd, (const char *) buffer, strlen(buffer),
                   MSG_CONFIRM, (const struct sockaddr *) svraddr,
                   len);


        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Receive response from server
void await_responses(int sockfd,struct rqst_udp_pkt * rqst_pkt,int expected_number,int res_image_size) {

    // expected image number
    // dump all packets which are not expected
    // we know how many packets for a completed image wait for those
    // if time_out request resend... this should not happen on a single machine

    int image_number, n;
    unsigned char buffer[MANDY_MAX];  //16K
    int len = sizeof(struct sockaddr_un);

    rgb_image_t *image;
    image = malloc(sizeof(rgb_image_t));
    image->image_size_x=res_image_size;
    image->image_size_y=res_image_size;
    image->image_data = malloc(image->image_size_x * image->image_size_y * 3);


    while (expected_number-- > 0) {
        n = recvfrom(sockfd, (char *) buffer, MANDY_MAX,
                     MSG_WAITALL, (struct sockaddr *) rqst_pkt->uxds_svraddr,
                     &len);
//        printf("\n Data-->");
//        printf("%d,%d,%d,%d %d %d:\n",
//               ((int *) buffer)[0], ((int *) buffer)[1],
//               ((int *) buffer)[2], ((int *) buffer)[3],
//               ((int *) buffer)[4], ((int *) buffer)[5]
//        );
        int res_image_number, r_start, i_start, n_real, n_imaginary;
        res_image_number = ((int *) buffer)[0];
        r_start = ((int *) buffer)[1];
        i_start = ((int *) buffer)[2];
        n_real = ((int *) buffer)[3];
        n_imaginary = ((int *) buffer)[4];
        int pkt_counter = 6 * sizeof(int); // first byte after header
        for (int r = 0; r < n_real; r++) {
            for (int i = 0; i < n_imaginary; i++) {

                int pixel_location = (i_start + r) * res_image_size * 3 + (r_start + i) * 3;
                //printf("%d,",pixel_location);
                image->image_data[pixel_location + 0] = buffer[pkt_counter++];
                image->image_data[pixel_location + 1] = buffer[pkt_counter++];
                image->image_data[pixel_location + 2] = buffer[pkt_counter++];

            }
            //printf("\n");
        }
    }
    //if fd is a terminal write to file


    if (isatty(fileno(stdout))) {
        write_rgb_file((char *) filename, image);
    }
    else
    {
        write_rgb_pipe(image);
    }
    free_rgb_image(image);
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
            real_center = strtod(arg, (char **)NULL);
            break;
        }
        case 'e':
        {
            real_segments = (int) strtol(arg, (char **)NULL, 10);
            break;
        }
        case 'y':
        {
            imaginary_center = strtod(arg, (char **)NULL);
            break;
        }
        case 'm':
        {
            imaginary_segments = (int) strtol(arg, (char **)NULL, 10);
            break;
        }
        case 's':
        {
            scale = strtod(arg, (char **)NULL);
            break;
        }
        case 'n':
        {
            image_size = (int) strtol(arg, (char **)NULL, 10);
            break;
        }
        case 'f':
        {
            strcpy(filename, arg);
            break;
        }
        default:
            break;
    }
    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Driver code
int main(int argc, char **argv)
{
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Default filename
    filename[0] = '\0';
    strcpy(filename, "mandy.ppm");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Set up the socket
    int sockfd;

    struct rqst_udp_pkt * rqst_pkt= make_rqst();
    sockfd=open_uxds_server_socket("/tmp/UDSDGSRV",rqst_pkt);

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

    config_lookup_int(cf,"defaults.n", &image_size);
    config_lookup_float(cf,"defaults.scale", &scale);

    const char *fileN;
    config_lookup_string(cf,"defaults.filename", &fileN);
    strcpy(filename,fileN);


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Parse command line args
    struct argp_option options[] =
            {
                    {"real_center",'x',"NUM",0,"Mandelbrot real center"},
                    {"real_segments",'e',"NUM",0,"Mandelbrot real segments"},
                    {"imaginary_center",'y',"NUM",0,"Mandelbrot imaginary center"},
                    {"imaginary_segments",'m',"NUM",0,"Mandelbrot imaginary segments"},
                    {"scale",'s',"NUM",0,"Mandelbrot scale"},
                    {"n",'n',"NUM",0,"Mandelbrot image size"},
                    {"filename",'f',0,0,"Name output ppm file"},
                    { 0 }
            };
    struct argp argp = { options, parse_opt,0,0};
    argp_parse(&argp,argc,argv,0,0,0);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Send requests to server and await the responses
    //printf("\n%f, %f, %d, %d, %d, %d\n",real_center,imaginary_center,real_segments,imaginary_segments,image_size,scale);
    complex double center = real_center+imaginary_center*I;
    int expected_number = real_segments * imaginary_segments;

    send_requests(sockfd,(struct sockaddr *) rqst_pkt->uxds_svraddr,10,center,scale,real_segments,imaginary_segments,image_size);
    await_responses(sockfd, rqst_pkt,expected_number,image_size);

    close(sockfd);
    return 0;
}