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