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

    int ret = SetStringValue("dhcp.lan.leasetime", strcat(buf, "s"));

    if (ret != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/dnsmasq restart");

    return ret;
}

int GetGateway(dm_req_t *req, char *buf, int len)
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "3,", 2) == 0) {
            snprintf(buf, len, "%s", dhcpOptions[i] + 2);
            found = true;
            break;
        }
    }

    if (!found) buf[0] = '\0';

    return USP_ERR_OK;
}

int SetGateway(dm_req_t *req, char *buf)
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "3,", 2) == 0) {
            strncpy(strcat("3,", dhcpOptions[i]), buf, 63);
            found = true;
            break;
        }
    }

    if (!found) return USP_ERR_INTERNAL_ERROR;

    int ret = SetListValues("dhcp.lan.dhcp_option", dhcpOptions, count);

    if (ret != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/dnsmasq restart");

    return ret;
}

int GetSubnetMask(dm_req_t *req, char *buf, int len)
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "1,", 2) == 0) {
            snprintf(buf, len, "%s", dhcpOptions[i] + 2);
            found = true;
            break;
        }
    }

    if (!found) buf[0] = '\0';

    return USP_ERR_OK;
}

int SetSubnetMask(dm_req_t *req, char *buf)
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "1,", 2) == 0) {
            strncpy(strcat("1,", dhcpOptions[i]), buf, 63);
            found = true;
            break;
        }
    }

    if (!found) return USP_ERR_INTERNAL_ERROR;

    int ret = SetListValues("dhcp.lan.dhcp_option", dhcpOptions, count);

    if (ret != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/dnsmasq restart");

    return ret;
}