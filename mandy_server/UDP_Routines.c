//
// Created by joel on 10/27/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <pthread.h>
#include "UDP_Routines.h"
extern int gsockfd;

void *send_response(void  *rqst){
    char *hello = (char *)malloc(40);

    // ToDo
    // The request will have the region of interest
    // calc mandy of the region get the RGB and send it back.
    /*
     rgb_image_t * calculate_mandelbrot(double complex center,
                                        double scale,
                                        unsigned int n,
                                        unsigned int max_iteration)
    */
    //  send the rgb_image_t data in the packet

    char *mandy_data = ((struct rqst_udp_pkt *)rqst)->rqst_data;

    sprintf(hello,"Hello from server %d",((struct rqst_udp_pkt *)rqst)->number);

    // Note: In our multi-threaded server we need unique buffers for each thread so need to malloc
    sleep(rand()%5);
    struct sockaddr_in * cliaddr = ((struct rqst_udp_pkt *)rqst)->cliaddr;
    sendto(gsockfd, (const char *)hello, strlen(hello),
           MSG_CONFIRM, (const struct sockaddr *) cliaddr,
           ((struct rqst_udp_pkt *)rqst)->len);

    printf("Hello message sent.\n");

}

void await_request(struct rqst_udp_pkt * rqst)
{
    rqst->len = MAXLINE;

    int n;
    n = recvfrom(gsockfd, (char *)rqst->rqst_data, MAXLINE,
                 MSG_WAITALL, ( struct sockaddr *) rqst->cliaddr,
                 &(rqst->len));
    rqst->rqst_data[n] = '\0';
    printf("Client : %s\n", rqst->rqst_data);
}

struct rqst_udp_pkt * make_rqst()
{
    struct rqst_udp_pkt *rqst = (struct rqst_udp_pkt *) malloc(sizeof(struct rqst_udp_pkt));
    rqst -> cliaddr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr));
    rqst -> rqst_data = (char *) malloc(MAXLINE);
    return rqst;
}

int open_inet_udp_socket(int port)
{
    struct sockaddr_in servaddr, *cliaddr;
    static int count=0;
    int sockfd;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *) &servaddr,
             sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}