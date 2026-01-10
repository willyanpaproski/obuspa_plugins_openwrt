#include "dhcpv4.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <string.h>
#include <stdio.h>

int GetLeaseTime(dm_req_t *req, char *buf, int len)
{
    (void)req;

    int err = GetStringValue("dhcp.lan.leasetime", "0", buf, len);
    if (err != USP_ERR_OK) {
        return err;
    }

    size_t str_len = strlen(buf);
    if (str_len > 0 && buf[str_len - 1] == 's') {
        buf[str_len - 1] = '\0';
    }

    return USP_ERR_OK;
}