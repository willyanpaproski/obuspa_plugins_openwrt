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
            snprintf(dhcpOptions[i], 64, "3,%s", buf);
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
            snprintf(dhcpOptions[i], 64, "1,%s", buf);
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

int GetMinAddress(dm_req_t *req, char *buf, int len)
{
    char dhcpOptions[16][64] = {0};
    char minIpAddressOctet[64];
    char *gatewayIp = NULL;
    int count = 0;
    bool found = false;
    
    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "3,", 2) == 0) {
            gatewayIp = dhcpOptions[i] + 2;
            found = true;
            break;
        }
    }

    if (!found) return USP_ERR_INTERNAL_ERROR;

    if (GetStringValue("dhcp.lan.start", minIpAddressOctet, sizeof(minIpAddressOctet)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    char *lastDot = strrchr(gatewayIp, '.');

    if (lastDot != NULL) {
        int prefixLen = (int)(lastDot - gatewayIp) + 1;       
        snprintf(buf, len, "%.*s%s", prefixLen, gatewayIp, minIpAddressOctet);
    } else {
        return USP_ERR_INTERNAL_ERROR;
    }

    return USP_ERR_OK;
}

int GetMaxAddress(dm_req_t *req, char *buf, int len) 
{
    char dhcpOptions[16][64] = {0};
    char maxIpAddressOctet[64];
    char *gatewayIp = NULL;
    int count = 0;
    bool found = false;
    
    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "3,", 2) == 0) {
            gatewayIp = dhcpOptions[i] + 2;
            found = true;
            break;
        }
    }

    if (!found) return USP_ERR_INTERNAL_ERROR;

    if (GetStringValue("dhcp.lan.limit", maxIpAddressOctet, sizeof(maxIpAddressOctet)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    char *lastDot = strrchr(gatewayIp, '.');

    if (lastDot != NULL) {
        int prefixLen = (int)(lastDot - gatewayIp) + 1;       
        snprintf(buf, len, "%.*s%s", prefixLen, gatewayIp, maxIpAddressOctet);
    } else {
        return USP_ERR_INTERNAL_ERROR;
    }

    return USP_ERR_OK;
}

int GetDnsServers(dm_req_t *req, char *buf, int len)
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "6,", 2) == 0) {
            snprintf(buf, len, "%s", dhcpOptions[i] + 2);
            found = true;
            break;
        }
    }

    if (!found) buf[0] = '\0';

    return USP_ERR_OK;
}

int SetDnsServers(dm_req_t *req, char *buf)
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "6,", 2) == 0) {
            snprintf(dhcpOptions[i], 64, "6,%s", buf);
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

int GetDomainName(dm_req_t *req, char *buf, int len) 
{
    char dhcpOptions[16][64] = {0};
    int count = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "15,", 2) == 0) {
            snprintf(buf, len, "%s", dhcpOptions[i] + 2);
            found = true;
            break;
        }
    }

    if (!found) buf[0] = '\0';

    return USP_ERR_OK;
}

int SetDomainName(dm_req_t *req, char *buf)
{
    char dhcpOptions[16][64] = {0};
    char newOptions[16][64] = {0};
    int count = 0;
    int newCount = 0;
    bool found = false;

    if (GetListValues("dhcp.lan.dhcp_option", dhcpOptions, 16, 64, &count) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    for (int i = 0; i < count; i++) {
        if (strncmp(dhcpOptions[i], "15,", 3) == 0) {
            if (buf != NULL && buf[0] != '\0') {
                snprintf(newOptions[newCount++], 64, "15,%s", buf);
                found = true;
            }
        } else {
            strncpy(newOptions[newCount++], dhcpOptions[i], 63);
        }
    }

    if (!found && buf != NULL && buf[0] != '\0') {
        snprintf(newOptions[newCount++], 64, "15,%s", buf);
    }

    if (SetListValues("dhcp.lan.dhcp_option", newOptions, newCount) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    system("/etc/init.d/dnsmasq restart");
    return USP_ERR_OK;
}

int ValidateAddPool(dm_req_t *req)
{
    return USP_ERR_OBJECT_NOT_CREATABLE;
}

int ValidateRemovePool(dm_req_t *req)
{
    return USP_ERR_OBJECT_NOT_DELETABLE;
}