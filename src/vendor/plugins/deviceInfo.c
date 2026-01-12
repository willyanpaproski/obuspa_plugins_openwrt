#include "deviceInfo.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <string.h>
#include <stdio.h>

int GetHostname(dm_req_t *req, char *buf, int len)
{
    (void)req;

    return GetStringValue("system.@system[0].hostname", buf, len);
}

int SetHostname(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    int ret = SetStringValue("system.@system[0].hostname", buf);

    if (ret == USP_ERR_OK) system("/etc/init.d/system restart");

    return ret;
 }