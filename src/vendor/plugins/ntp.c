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

int SetNTPServer1(dm_req_t *req, char *buf)
{
    char servers[16][64] = {0};
    int count = 0;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    strncpy(servers[0], buf, 63);
    
    return SetListValues("system.ntp.server", servers, count);
}

int GetNTPServer2(dm_req_t *req, char *buf, int len)
{
    return GetNtpServerByIndex(1, buf, len);
}

int SetNTPServer2(dm_req_t *req, char *buf)
{
    char servers[16][64] = {0};
    int count = 1;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    strncpy(servers[1], buf, 63);
    
    return SetListValues("system.ntp.server", servers, count);
}