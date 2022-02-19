//
// Created by al1ve1t on 11.12.21.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "server/server.h"
#include "zlog.h"
#include "globals.h"

#define ZLOG_CONF_PATH "/etc/zlog.conf"

int main(int argc, char *argv[])
{
    int rc;

    rc = zlog_init(ZLOG_CONF_PATH);
    if (rc)
    {
        printf("ZLog init failed\n");
        return -1;
    }
    z_category = zlog_get_category("default");

    RunServer();
    return 0;
}
