#include "ntp.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <stdio.h>

int GetNTPServer1(dm_req_t *req, char *buf, int len)
{
    char servers[16][64];
    int count = 0;
    
    GetListValues("system.ntp.server", servers, 16, 64, &count);

    if (count > 0) {
        snprintf(buf, len, "%s", servers[0]);
    } else {
        buf[0] = '\0';
    }

    return USP_ERR_OK;
}

int GetNTPServer2(dm_req_t *req, char *buf, int len)
{
    char servers[16][64];
    int count = 0;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    if (count > 0) {
        snprintf(buf, len, "%s", servers[0]);
    } else {
        buf[0] = '\0';
    }

    return USP_ERR_OK;
}