#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/net.h>

#include "zlog.h"

#define BUF_SIZE 0xFF

sem_t src_sem;
sem_t dst_sem;

uint8_t src_buf[BUF_SIZE];
uint8_t dst_buf[BUF_SIZE];

bool RetranslateBytes(int srcsock, int dstsock)
{
    sem_init(&src_sem, 0, 1);
    sem_init(&dst_sem, 0, 1);

    pthread_t sts, rfs, std, rfd; // all 4 communications name-encoded

    // Run communication threads
    pthread_create(&rfs, NULL, ReceiveFromSource, (void *)srcsock);
    pthread_create(&std, NULL, SendToDestination, (void *)dstsock);
    pthread_create(&rfd, NULL, ReceiveFromDestination, (void *)dstsock);
    pthread_create(&sts, NULL, SendToSource, (void *)srcsock);

    // Close communication threads
    pthread_join(rfs, NULL);
    pthread_join(std, NULL);
    pthread_join(rfd, NULL);
    pthread_join(sts, NULL);
}

void *SendToSource(int srcsock)
{
}

void *ReceiveFromSource(int srcsock)
{
    int bytes_recvd;
    do
    {
        sem_wait(&src_sem);
        bytes_recvd = recv(srcsock, src_buf, BUF_SIZE, 0);
        sem_post(&src_sem);
    } while (bytes_recvd > 0)
}

void *SendToDestination(int dstsock)
{
}

void *ReceiveFromDestination(int dstsock)
{
    int bytes_recvd;
    do
    {
        sem_wait(&dst_sem);
        bytes_recvd = recv(dstsock, dst_buf, BUF_SIZE, 0);
        sem_post(&dst_sem);
    } while (bytes_recvd > 0)
}
