#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    int socket_fd;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    int bytes_received, bytes_sent;
    int integer_buffer;
    socklen_t address_length = sizeof(struct sockaddr_un);

    if((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("test_server_UDS: socket");
        return 1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, "/tmp/UDSDGSRV");

    unlink("/tmp/UDSDGSRV");
    if(bind(socket_fd, (const struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        close(socket_fd);
        perror("test_server_UDS: bind");
        return 1;
    }

    while(1)
    {
        /* address_length is the length of the client's socket address structure.
           Hear this should always be the same since these socets are of type struct sockaddr_un.
           However, code that could be used with different types of sockets, ie UDS and UPD should
           take care to hold and pass the correct value back to sendto on reply.  */

        bytes_received = recvfrom(socket_fd, (char *) &integer_buffer, sizeof(int), 0,
                                  (struct sockaddr *) &(client_address),
                                  &address_length);

        if(bytes_received != sizeof(int))
        {
            printf("datagram was the wrong size.\n");
        } else {
            integer_buffer += 5;

            bytes_sent = sendto(socket_fd, (char *) &integer_buffer, sizeof(int), 0,
                                (struct sockaddr *) &(client_address),
                                address_length);
        }
    }

    unlink("~/UDSDGSRV");
    close(socket_fd);

    return 0;
}
