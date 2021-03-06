//
// Created by al1ve1t on 11.12.21.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netdb.h>
#include <pthread.h>
#include <linux/net.h>
#include "zlog.h"
#include "server.h"

zlog_category_t *z_category;

#define DEFAULT_PORT 1080
#define BUF_SIZE 0xFF

#ifndef PORT
#define PORT DEFAULT_PORT
#endif

// SOCKS5 AUTH METHODS
#define NO_AUTH 0x00
#define GSSAPI 0x01
#define USER_PASS 0x02

void *Handshake(void *connfd);

void RunServer()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    // Create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        zlog_fatal(z_category, "Could not create socket.");
        exit(1);
    }
    else
        zlog_info(z_category, "Socket creation success.");
    bzero(&serveraddr, sizeof(serveraddr));

    // Assign IP and PORT
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serveraddr.sin_port = htons(PORT);

    // Bind
    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0)
    {
        zlog_fatal(z_category, "Could not bind socket.");
        exit(2);
    }
    else
        zlog_info(z_category, "Socket binding success.");

    // Listen
    if (listen(sockfd, 5) != 0)
    {
        zlog_fatal(z_category, "Socket listen failed.");
        exit(3);
    }
    else
        zlog_info(z_category, "Listening started..");

    // Accept
    for (;;)
    {
        uint64_t connfd = accept(sockfd, NULL, NULL);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, Handshake, (void *)connfd);
        // pthread_join(thread_id, NULL);
    }
    close(sockfd);
}

bool IsNoAuthAllowed(uint8_t *methods, uint8_t n_methods)
{
    for (int i = 0; i < n_methods; ++i)
        if (methods[i] == NO_AUTH)
            return true;
}

uint8_t ChooseAuth(uint8_t *methods, uint8_t n_methods)
{
    if (IsNoAuthAllowed(methods, n_methods))
        return NO_AUTH;
    for (int i = 0; i < n_methods; ++i)
        if (methods[i] && (GSSAPI || USER_PASS))
            return methods[i];
    return 0xFF;
}

void *Handshake(void *connfd)
{
    uint64_t sock = (uint64_t)connfd;
    zlog_info(z_category, "Routine started.");
    uint8_t buf[BUF_SIZE];
    int bytes_recvd = 0;

    // Initial greetings
    uint8_t socks_v;
    uint8_t n_auth_methods;
    uint8_t *auth_methods;

    // Read bytes
    bytes_recvd = recv(sock, buf, BUF_SIZE, 0);
    socks_v = buf[0];
    n_auth_methods = buf[1];
    auth_methods = &buf[2];

    // Choose auth method
    uint8_t method = ChooseAuth(auth_methods, n_auth_methods);

    // Send choosed auth method
    bzero(&buf, BUF_SIZE);
    buf[0] = socks_v;
    buf[1] = method;
    send(sock, buf, 2, 0); // send 2 bytes response

    // Read request details
    bytes_recvd = recv(sock, buf, BUF_SIZE, 0);
    size_t host_len = buf[4];
    uint8_t *host_name = malloc(host_len);
    short port_sh;

    memcpy(host_name, &buf[5], host_len);
    struct hostent *dest_host = gethostbyname((char *)host_name);
    memcpy(&port_sh, &buf[bytes_recvd - 2], 2);
    uint16_t dest_port = ntohs(port_sh);

    // Connect to destination host
    int dest_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (dest_sock == -1)
    {
        zlog_fatal(z_category, "Could not create destination socket.");
        exit(1);
    }
    struct sockaddr_in dest_addr;
    bzero(&dest_addr, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(dest_port);
    bcopy((uint8_t *)dest_host->h_addr_list[0], (uint8_t *)&dest_addr.sin_addr.s_addr,
          dest_host->h_length);

    if (connect(dest_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        zlog_fatal(z_category, "Destination host: connection failed.");
        // Send 0x04 to SOCKS5 client
    }
    // Send connection status to SOCKS5 client
    bzero(&buf, BUF_SIZE);
    buf[0] = socks_v;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x03;
    buf[4] = host_len;
    bcopy(host_name, &buf[5], host_len);
    dest_port = htons(dest_port);
    bcopy(&dest_port, &buf[5 + host_len], 2);
    send(sock, buf, 5 + host_len + 2, 0);

    // Retranslation of bytes begins here.

    if (!RetranslationBegin(sock, dest_sock))
    {
    }

    free(host_name);
    close(sock);
    close(dest_sock);
}

char *Print()
{
    return "Hello, World!\n";
}
