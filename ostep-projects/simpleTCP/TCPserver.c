// TCPserver.c: simple server, basis found on geeksforgeeks.org
//              adapted to handle concurrent requests
// usage:  >./TCPserver
//         >./TCPclient 
//TCPclient> <chatty words>
#include <netinet/in.h>
#include <sys/types.h>

#include "common.h"

// simple function for chat between client and server 
void chatfun(int sockfd)
{
    char buff[MAX];
    int n;

    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy to buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // send that buffer to client
        write(sockfd, buff, sizeof(buff));

        // if message contains "Exit" then server exit and char ended. 
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main() 
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed ...\n");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else {
        printf("Success! Socket bound..\n");
    }

    // Now server is ready to listen and verify
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed..\n");
        exit(0);
    } else {
        printf("Server listening...\n");
    }
    len = sizeof(cli);

    // Accept the data packet from client and verify
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
    } else {
        printf("server accepts client request...\n");
    }

    // function for chatting between client and server
    chatfun(connfd);

    // after chatting close the socket
    close(sockfd);

    return 0;
}
