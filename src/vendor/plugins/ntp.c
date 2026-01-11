#include "ntp.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <stdio.h>

int GetNtpServerAddress(dm_req_t *req, char *buf, int len)
{
    char servers[8][64];
    int count = 0;
    int index;

    if (req == NULL ||
        req->inst == NULL ||
        req->inst->order < 1)
    {
        buf[0] = '\0';
        return USP_ERR_OK;
    }

    GetListValues("system.ntp.server", servers, 8, 64, &count);

    index = req->inst->instances[0] - 1;

    if (index < 0 || index >= count) {
        buf[0] = '\0';
        return USP_ERR_OK;
    }

    snprintf(buf, len, "%s", servers[index]);
    return USP_ERR_OK;
}
