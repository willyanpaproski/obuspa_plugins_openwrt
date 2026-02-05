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

    if (SetListValues("system.ntp.server", servers, count) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;
    
    system("/etc/init.d/ntpd restart");
    system("/etc/init.d/ntpdate restart");
    system("/etc/init.d/sysntpd restart");

    return USP_ERR_OK;
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

    if (SetListValues("system.ntp.server", servers, count) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/ntpd restart");
    system("/etc/init.d/ntpdate restart");
    system("/etc/init.d/sysntpd restart");
    
    return USP_ERR_OK;
}

int GetNTPEnabled(dm_req_t *req, char *buf, int len)
{
    char enableVal[64];

    if (GetStringValue("system.ntp.enabled", enableVal, sizeof(enableVal)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    if (strcmp(enableVal, "1") == 0) {
        snprintf(buf, len, "false");
    } else {
        snprintf(buf, len, "true");
    }

    return USP_ERR_OK;
}

int SetNTPEnabled(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    char *uciValue;

    if (strcmp(buf, "true") == 0) uciValue = "0";

    else if (strcmp(buf, "false") == 0) uciValue = "1";

    else return USP_ERR_INTERNAL_ERROR;

    if (SetStringValue("system.ntp.enabled", uciValue) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/ntpd restart");
    system("/etc/init.d/ntpdate restart");
    system("/etc/init.d/sysntpd restart");

    return USP_ERR_OK;
}