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

int SetLeaseTime(dm_req_t *req, char *buf) 
{  
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    char fullValue[32];

    snprintf(fullValue, sizeof(fullValue), "%ss", buf);

    int ret = SetStringValue("dhcp.lan.leasetime", fullValue);

    if (ret != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/dnsmasq restart");
    return ret;
}

int GetGateway(dm_req_t *req, char *buf, int len)
{
    (void)req;

    return GetStringValue("network.lan.gateway", buf, len);
}

int SetGateway(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    int ret = SetStringValue("network.lan.gateway", buf);

    if (ret == USP_ERR_OK) system("/etc/init.d/dnsmasq restart");

    return ret;
}

int GetSubnetMask(dm_req_t *req, char *buf, int len)
{
    (void)req;

    return GetStringValue("network.lan.netmask", buf, len);
}

int SetSubnetMask(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    int ret = SetStringValue("network.lan.netmask", buf);

    if (ret == USP_ERR_OK) system("/etc/init.d/dnsmasq restart");

    return ret;
}

int ValidateAddPool(dm_req_t *req)
{
    return USP_ERR_OBJECT_NOT_CREATABLE;
}

int ValidateRemovePool(dm_req_t *req)
{
    return USP_ERR_OBJECT_NOT_DELETABLE;
}