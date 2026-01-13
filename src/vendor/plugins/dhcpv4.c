#include "dhcpv4.h"
#include "utils.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <string.h>
#include <stdio.h>

int GetLeaseTime(dm_req_t *req, char *buf, int len)
{
    char uciVal[64];
    int numericVal;

    if (GetStringValue("dhcp.lan.leasetime", uciVal, sizeof(uciVal)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    numericVal = atoi(uciVal);

    if (endsWith(uciVal, "h")) {
        numericVal = numericVal * 3600; 
    } else if (endsWith(uciVal, "m")) {
        numericVal = numericVal * 60;
    }

    snprintf(buf, len, "%d", numericVal);

    return USP_ERR_OK;
}

int GetGateway(dm_req_t *req, char *buf, int len)
{
    char gateway[32];
    
    if (GetStringValue("get network.lan.ipaddr", gateway, sizeof(gateway)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    char *slash = strchr(gateway, '/');

    if (slash != NULL) {
        *slash = '\0';
    }

    snprintf(buf, len, "%s", gateway);

    return USP_ERR_OK;
}