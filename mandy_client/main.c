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

#define PORT	 8080
#define MAXLINE 1024

double real_center;
int real_segments;

double imaginary_center;
int imaginary_segments;

int n;
int scale;

const char* filename;

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
    struct sockaddr_in	 servaddr;
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

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
    printf("%f %d %f %d %d %d %s",real_center, real_segments, imaginary_center, imaginary_segments, scale, n, filename);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    int num_bytes_read, len;

    //Issue requests to the server based on arguments
    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");

    sendto(sockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) &servaddr,
           sizeof(servaddr));
    printf("Hello message sent.\n");


    //Receive incoming responses from the server.
    num_bytes_read = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,
                 &len);
    buffer[num_bytes_read] = '\0';
    printf("Server : %s\n", buffer);

    num_bytes_read = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *) &servaddr,
                 &len);
    buffer[num_bytes_read] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}