//
// Created by al1ve1t on 18.02.22.
//

#include "zlog.h"
#include "../../globals.h"

typedef struct
{
    int sockfd;
    sem_t *mutex;
} sock_t;

typedef struct
{
    sock_t *fromfd;
    sock_t *tofd;
    uint8_t *buf;
} th_args;

static void *Retranslate(void *args);
static th_args MakeArgs(sock_t *fromfd, sock_t *tofd, uint8_t *buf);
static sock_t MakeSock(int sockfd, sem_t *mutex);
SSL_CTX *CreateCtx();
void ConfigureCtx(SSL_CTX *ctx);
