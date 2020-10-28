//
// Created by joel on 10/27/20.
//

#ifndef UDPSERVER_UDP_ROUTINES_H
#define UDPSERVER_UDP_ROUTINES_H


#define MAXLINE  1024

struct rqst_udp_pkt{
    int number;
    struct sockaddr_in * cliaddr;
    int len;
    char *rqst_data;
};
void *send_response(void  * rqst);
void await_request(struct rqst_udp_pkt * rqst);
struct rqst_udp_pkt * make_rqst();
int open_inet_udp_socket(int port);

#endif //UDPSERVER_UDP_ROUTINES_H
