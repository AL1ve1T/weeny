//
// Created by al1ve1t on 18.02.22.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <linux/net.h>
#include <openssl/ssl.h>
#include <semaphore.h>
#include "zlog.h"
#include "retranslator.h"

#define BUF_SIZE 0x1FFF

uint8_t src_buf[BUF_SIZE];
uint8_t dst_buf[BUF_SIZE];

extern bool RetranslationBegin(int src_fd, int dst_fd)
{
    // both communications name-encoded
    pthread_t rtd;
    pthread_t rts;

    // Construct args for both threads
    sem_t src_mutex;
    sem_t dst_mutex;
    sem_init(&src_mutex, 0, 1);
    sem_init(&dst_mutex, 0, 1);

    sock_t src_sock = MakeSock(src_fd, &src_mutex);
    sock_t dst_sock = MakeSock(dst_fd, &dst_mutex);

    th_args src_args = MakeArgs(&src_sock, &dst_sock, src_buf);
    th_args dst_args = MakeArgs(&dst_sock, &src_sock, dst_buf);

    // Run communication threads
    pthread_create(&rtd, NULL, Retranslate, (void *)&src_args);
    pthread_create(&rts, NULL, Retranslate, (void *)&dst_args);

    // Close communication threads
    pthread_join(rtd, NULL);
    pthread_join(rts, NULL);

    sem_destroy(&src_mutex);
    sem_destroy(&dst_mutex);
}

static int LockAndRecv(sock_t *sock, uint8_t *buf)
{
    // sem_wait(sock->mutex);
    int bytes_recvd = recv(sock->sockfd, buf, BUF_SIZE, MSG_CMSG_CLOEXEC);
    // sem_post(sock->mutex);
    return bytes_recvd;
}

static int LockAndSend(sock_t *sock, uint8_t *buf, int numToSend)
{
    // sem_wait(sock->mutex);
    int bytes_sent = send(sock->sockfd, buf, numToSend, 0);
    // sem_post(sock->mutex);
    return bytes_sent;
}

static void *Retranslate(void *args)
{
    // Unpack arguments
    th_args *args_ = (th_args *)args;

    sock_t *fromfd = args_->fromfd;
    sock_t *tofd = args_->tofd;
    uint8_t *buf = args_->buf;

    int bytes_recvd = 0;
    int bytes_sent = 0;
    do
    {
        bool stop = false;
        bytes_recvd = LockAndRecv(fromfd, buf);
        if (bytes_recvd < BUF_SIZE)
            stop = true;

        bytes_sent = LockAndSend(tofd, buf, bytes_recvd);

        if (stop)
            break;
        bzero(buf, BUF_SIZE);
    } while (bytes_recvd > 0);
}

static th_args MakeArgs(sock_t *fromfd, sock_t *tofd, uint8_t *buf)
{
    return (th_args){.fromfd = fromfd, .tofd = tofd, .buf = buf};
}

static sock_t MakeSock(int sockfd, sem_t *mutex)
{
    return (sock_t){.sockfd = sockfd, .mutex = mutex};
}

// https://wiki.openssl.org/index.php/Simple_TLS_Server
/*SSL_CTX *CreateCtx()
{
    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx)
        zlog_fatal(z_category, "SSL context creation failed.");
    return ctx;
}

void ConfigureCtx(SSL_CTX *ctx)
{
    if ((SSL_CTX_use_certificate_file(ctx, "*.pem", SSL_FILETYPE_PEM) < 1) ||
        (SSL_CTX_use_PrivateKey_file(ctx, "*.pem", SSL_FILETYPE_PEM) < 1))
        zlog_fatal(z_category, "SSL configuration failed.");
}
*/