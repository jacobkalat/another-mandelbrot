//https://www.geeksforgeeks.org/udp-server-client-implementation-c/
// Server side implementation of UDP client-server model

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "UDP_Routines.h"

int gsockfd;

// Driver code
int main() {

    static int count=0;

    pthread_t tid;
    gsockfd=open_uxds_udp_socket();

    while (1) {
        //  await request
        struct rqst_udp_pkt *rqst=make_rqst();
        rqst->number=count;
        await_request(rqst);
        pthread_create(&tid, NULL, send_response, (void *) rqst);
        count++;
    }
    pthread_join(tid,NULL);  // should wait for all threads to terminate.
    return 0;
}