#include "dhcpv4.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <string.h>
#include <stdio.h>

int GetLeaseTime(dm_req_t *req, char *buf, int len)
{
    (void)req;
    char raw_value[64];
    
    int err = GetStringValue("dhcp.lan.leasetime", "12h", raw_value, sizeof(raw_value));
    if (err != USP_ERR_OK) {
        return err;
    }

    char *endptr;
    long value = strtol(raw_value, &endptr, 10);
    long seconds = value;

    if (*endptr != '\0') {
        switch (*endptr) {
            case 'm': seconds = value * 60; break;
            case 'h': seconds = value * 3600; break;
            case 'd': seconds = value * 86400; break;
            case 's': seconds = value; break;
            default:  seconds = value; break; 
        }
    }

    snprintf(buf, len, "%ld", seconds);

    return USP_ERR_OK;
}