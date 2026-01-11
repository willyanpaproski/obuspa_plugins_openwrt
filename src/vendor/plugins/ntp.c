#include "ntp.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <stdio.h>

static int GetNtpServerByIndex(int index, char *buf, int len)
{
    char servers[16][64] = {0}; 
    int count = 0;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    if (index >= 0 && index < count) {
        snprintf(buf, len, "%s", servers[index]);
    } else {
        buf[0] = '\0';
    }

    return USP_ERR_OK;
}

int GetNTPServer1(dm_req_t *req, char *buf, int len)
{
    return GetNtpServerByIndex(0, buf, len);
}

int GetNTPServer2(dm_req_t *req, char *buf, int len)
{
    return GetNtpServerByIndex(1, buf, len);
}