#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <linux/net.h>

#include "zlog.h"

#define BUF_SIZE 0x1FFF

uint8_t src_buf[BUF_SIZE];
uint8_t dst_buf[BUF_SIZE];

typedef struct
{
    int fromfd;
    int tofd;
    uint8_t *buf;
} th_args;

static void *Retranslate(void *args);
static th_args MakeArgs(int fromfd, int tofd, uint8_t *buf);

extern bool RetranslationBegin(int srcsock, int dstsock)
{
    // both communications name-encoded
    pthread_t rtd;
    pthread_t rts;

    // Run communication threads
    th_args src_args = MakeArgs(srcsock, dstsock, src_buf);
    th_args dst_args = MakeArgs(dstsock, srcsock, dst_buf);

    pthread_create(&rtd, NULL, Retranslate, (void *)&src_args);
    pthread_create(&rts, NULL, Retranslate, (void *)&dst_args);

    // Close communication threads
    pthread_join(rtd, NULL);
    pthread_join(rts, NULL);
}

static void *Retranslate(void *args)
{
    // Unpack arguments
    th_args *args_ = (th_args *)args;

    int fromfd = args_->fromfd;
    int tofd = args_->tofd;
    uint8_t *buf = args_->buf;

    int bytes_recvd = 0;
    int bytes_sent = 0;
    do
    {
        bool stop = false;
        bytes_recvd = recv(fromfd, buf, BUF_SIZE, MSG_CMSG_CLOEXEC);
        if (bytes_recvd < BUF_SIZE)
            stop = true;

        bytes_sent = send(tofd, buf, bytes_recvd, 0);

        if (stop)
            break;
        bzero(buf, BUF_SIZE);
    } while (bytes_recvd > 0);
}

static th_args MakeArgs(int fromfd, int tofd, uint8_t *buf)
{
    return (th_args){.fromfd = fromfd, .tofd = tofd, .buf = buf};
}