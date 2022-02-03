//
// Created by al1ve1t on 11.12.21.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "zlog.h"

#define DEFAULT_PORT 1080

#ifndef PORT
#define PORT DEFAULT_PORT
#endif

#include <linux/net.h>

void RunServer()
{
    zlog_category_t *c;
    c = zlog_get_category("default");

    struct sockaddr_in serveraddr;
    int sockfd;
    // Create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        zlog_fatal(c, "Could not create socket.");
        exit(1);
    }
    else
    {
        zlog_info(c, "Socket creation success.");
    }
    bzero(&serveraddr, sizeof(serveraddr));

    // Assign IP and PORT
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    // Bind
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0)
    {
        zlog_fatal(c, "Could not bind socket.");
        exit(2);
    }
    else
    {
        zlog_info(c, "Socket binding success.");
    }

    // Listen
    if (listen(sockfd, 5) != 0)
    {
        zlog_fatal(c, "Socket listen failed.");
        exit(3);
    }
    else
    {
        zlog_info(c, "Listening started..");
    }

    // Accept
}

char *Print()
{
    return "Hello, World!\n";
}