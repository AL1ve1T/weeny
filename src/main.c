//
// Created by al1ve1t on 11.12.21.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "server/server.h"
#include "zlog.h"

int main(int argc, char *argv[])
{
    int rc;

    rc = zlog_init("/etc/zlog.conf");
    if (rc)
    {
        printf("ZLog init failed\n");
        return -1;
    }

    RunServer();
    return 0;
}
