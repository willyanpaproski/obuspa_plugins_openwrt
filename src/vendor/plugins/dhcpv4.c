#include "dhcpv4.h"
#include "utils.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int GetLastOctet(char *ip) {
    if (!ip) return -1;
    char *lastDot = strrchr(ip, '.');
    if (!lastDot) return -1;
    return atoi(lastDot + 1);
}

int GetDHCPv4Params(int group_id, kv_vector_t *params)
{
    char buf[256];

    for (int i = 0; i <params->num_entries; i++)
    {
        kv_pair_t *kv = &params->vector[i];

        if (strcmp(kv->key, "Device.DHCPv4.Server.Enable") == 0)
        {
            GetPoolEnabled(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.PoolNumberOfEntries") == 0)
        {
            snprintf(buf, sizeof(buf), "1");
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.Alias") == 0)
        {
            snprintf(buf, sizeof(buf), "cpe-dhcpv4pool");
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.DNSServers") == 0)
        {
            GetDnsServers(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.DomainName") == 0)
        {
            GetDomainName(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.Enable") == 0)
        {
            GetPoolEnabled(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.IPRouters") == 0)
        {
            GetGateway(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.LeaseTime") == 0)
        {
            GetLeaseTime(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.SubnetMask") == 0)
        {
            GetSubnetMask(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.MaxAddress") == 0)
        {
            GetMaxAddress(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.MinAddress") == 0)
        {
            GetMinAddress(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DHCPv4.Server.Pool.1.Status") == 0)
        {
            GetDHCPv4Status(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }
    }

    return USP_ERR_OK;
}

int SetDHCPv4Params(int group_id, kv_vector_t *params, unsigned *types, int *failure_index)
{
    bool needs_restart = false;
    int err = USP_ERR_OK;

    for (int i = 0; i < params->num_entries; i++)
    {
        char *key = params->vector[i].key;
        char *val = params->vector[i].value;

        if (strcmp(key, "Device.DHCPv4.Server.Enable") == 0)
        {
            err = SetPoolEnabled(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.DNSServers") == 0)
        {
            err = SetDnsServers(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.DomainName") == 0)
        {
            err = SetDomainName(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.Enable") == 0)
        {
            err = SetPoolEnabled(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.IPRouters") == 0)
        {
            err = SetGateway(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.LeaseTime") == 0)
        {
            err = SetLeaseTime(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.SubnetMask") == 0)
        {
            err = SetSubnetMask(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.MaxAddress") == 0)
        {
            err = SetMaxAddress(NULL, val);
        }

        if (strcmp(key, "Device.DHCPv4.Server.Pool.1.MinAddress") == 0)
        {
            err = SetMinAddress(NULL, val);
        }

        if (err != USP_ERR_OK)
        {
            return err;
        }

        needs_restart = true;
    }

    if (needs_restart)
    {
        system("/etc/init.d/dnsmasq restart");
    }

    return USP_ERR_OK;
}

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

    if (buf == NULL || buf[0] == '\0') return USP_ERR_INTERNAL_ERROR;

    if (!isValidIPv4Address(buf)) return USP_ERR_INVALID_VALUE;

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

    if (buf == NULL || buf[0] == '\0') return USP_ERR_INTERNAL_ERROR;

    if (!isValidIPv4Address(buf)) return USP_ERR_INVALID_VALUE;

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

int SetMinAddress(dm_req_t *req, char *buf)
{
    if (buf == NULL || buf[0] == '\0') return USP_ERR_INTERNAL_ERROR;

    if (!isValidIPv4Address(buf)) return USP_ERR_INVALID_VALUE;

    int octet = GetLastOctet(buf);
    if (octet < 0 || octet > 255) return USP_ERR_INVALID_VALUE;

    char value[8];
    snprintf(value, sizeof(value), "%d", octet);

    if (SetStringValue("dhcp.lan.start", value) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    system("/etc/init.d/dnsmasq restart");
    
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

int SetMaxAddress(dm_req_t *req, char *buf)
{
    if (buf == NULL || buf[0] == '\0') return USP_ERR_INTERNAL_ERROR;

    if (!isValidIPv4Address(buf)) return USP_ERR_INVALID_VALUE;

    int octet = GetLastOctet(buf);
    if (octet < 0 || octet > 255) return USP_ERR_INVALID_VALUE;

    char value[8];
    snprintf(value, sizeof(value), "%d", octet);

    if (SetStringValue("dhcp.lan.limit", value) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    system("/etc/init.d/dnsmasq restart");
    
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
        if (strncmp(dhcpOptions[i], "15,", 3) == 0) {
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

int GetPoolEnabled(dm_req_t *req, char *buf, int len)
{
    char enableVal[64];
    
    if (GetStringValue("dhcp.lan.ignore", enableVal, sizeof(enableVal)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    if (strcmp(enableVal, "1") == 0) {
        snprintf(buf, len, "false");
    } else {
        snprintf(buf, len, "true");
    }

    return USP_ERR_OK;
}

int SetPoolEnabled(dm_req_t *req, char *buf) 
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    char *uciValue;

    if (strcmp(buf, "true") == 0) uciValue = "0";

    else if (strcmp(buf, "false") == 0) uciValue = "1";

    else return USP_ERR_INVALID_VALUE;

    if (SetStringValue("dhcp.lan.ignore", uciValue) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    system("/etc/init.d/dnsmasq restart");

    return USP_ERR_OK;
}

bool isDnsMasqRunning() 
{
    DIR *dir = opendir("/var/run/dnsmasq");
    if (dir == NULL) return false;

    struct dirent *entry; 
    bool running = false;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, ".pid") != NULL) {
            running = true;
            break;
        }
    }

    closedir(dir);
    return running;
}

int GetDHCPv4Status(dm_req_t *req, char *buf, int len)
{
    char ignoreVal[16] = {0};

    if (GetStringValue("dhcp.lan.ignore", ignoreVal, sizeof(ignoreVal)) != USP_ERR_OK) {
        snprintf(buf, len, "Error");
        return USP_ERR_OK;
    }

    if (strcmp(ignoreVal, "1") == 0) {
        snprintf(buf, len, "Disabled");
        return USP_ERR_OK;
    }

    if (isDnsMasqRunning()) {
        snprintf(buf, len, "Enabled");
    } else {
        snprintf(buf, len, "Error");
    }

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