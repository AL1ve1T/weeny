//
// Created by al1ve1t on 11.12.21.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_PORT 1080

#ifndef PORT
#define PORT DEFAULT_PORT
#endif

void RunServer() {
    struct sockaddr_in serveraddr;
    int sockfd;
    // Create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        //failed
        exit(1);
    } else {
        //created
    }
    bzero(&serveraddr, sizeof(serveraddr));

    // Assign IP and PORT
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    // Bind
    if (bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) {
        // failed
    } else {
        // created
    }

    // Listen
    if (listen(sockfd, 5) != 0) {
        // failed
    } else {
        // success
    }

    // Accept

}

char* Print() {
    return "Hello, World!\n";
}