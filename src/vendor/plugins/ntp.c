#include "ntp.h"
#include "usp_err_codes.h"
#include "uciHelper.h"
#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <sys/timex.h>

static int GetNtpServerByIndex(int index, char *buf, int len)
{
    char servers[16][64] = {0}; 
    int count = 0;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    if (index >= 0 && index < count) {
        snprintf(buf, len, "%s", servers[index]);
    } else {
        buf[0] = '\0';
    }

    return USP_ERR_OK;
}

int SetTimeParams(int group_id, kv_vector_t *params, unsigned *types, int *failure_index)
{
    bool needs_restart = false;
    int err = USP_ERR_OK;

    for (int i = 0; i < params->num_entries; i++) 
    {
        char *key = params->vector[i].key;
        char *val = params->vector[i].value;

        if (strcmp(key, "Device.Time.Enable") == 0)
        {
            err = SetNTPEnabled(NULL, val);
        }

        else if (strcmp(key, "Device.Time.LocalTimeZone") == 0)
        {
            err = SetLocalTimeZone(NULL, val);
        }

        else if (strcmp(key, "Device.Time.NTPServer1") == 0)
        {
            err = SetNTPServer1(NULL, val);
        }

        else if (strcmp(key, "Device.Time.NTPServer2") == 0)
        {
            err = SetNTPServer2(NULL, val);
        }

        if (err != USP_ERR_OK)
        {
            return err;
        }

        needs_restart = true;
    }

    if (needs_restart)
    {
        system("/etc/init.d/ntpd restart");
        system("/etc/init.d/ntpdate restart");
        system("/etc/init.d/sysntpd restart");
    }

    return USP_ERR_OK;
}

int GetTimeParams(int group_id, kv_vector_t *params)
{
    char buf[256];

    for (int i = 0; i < params->num_entries; i++)
    {
        kv_pair_t *kv = &params->vector[i];

        if (strcmp(kv->key, "Device.Time.CurrentLocalTime") == 0) 
        {
            GetCurrentLocalTime(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        else if (strcmp(kv->key, "Device.Time.Enable") == 0)
        {
            GetNTPEnabled(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        else if (strcmp(kv->key, "Device.Time.LocalTimeZone") == 0)
        {
            GetLocalTimeZone(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        else if (strcmp(kv->key, "Device.Time.NTPServer1") == 0)
        {
            GetNTPServer1(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        else if (strcmp(kv->key, "Device.Time.NTPServer2") == 0)
        {
            GetNTPServer2(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        else if (strcmp(kv->key, "Device.Time.Status") == 0)
        {
            GetNTPStatus(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }
    }

    return USP_ERR_OK;
}

int GetNTPServer1(dm_req_t *req, char *buf, int len)
{
    return GetNtpServerByIndex(0, buf, len);
}

int SetNTPServer1(dm_req_t *req, char *buf)
{
    char servers[16][64] = {0};
    int count = 0;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    strncpy(servers[0], buf, 63);

    if (count < 1) count = 1;

    if (SetListValues("system.ntp.server", servers, count) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    return USP_ERR_OK;
}

int GetNTPServer2(dm_req_t *req, char *buf, int len)
{
    return GetNtpServerByIndex(1, buf, len);
}

int SetNTPServer2(dm_req_t *req, char *buf)
{
    char servers[16][64] = {0};
    int count = 0;

    GetListValues("system.ntp.server", servers, 16, 64, &count);

    strncpy(servers[1], buf, 63);

    if (count < 2) count = 2;

    if (SetListValues("system.ntp.server", servers, count) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;
    
    return USP_ERR_OK;
}

int GetNTPEnabled(dm_req_t *req, char *buf, int len)
{
    char enableVal[64];

    if (GetStringValue("system.ntp.enabled", enableVal, sizeof(enableVal)) != USP_ERR_OK) {
        return USP_ERR_INTERNAL_ERROR;
    }

    if (strcmp(enableVal, "1") == 0) {
        snprintf(buf, len, "true");
    } else {
        snprintf(buf, len, "false");
    }

    return USP_ERR_OK;
}

int SetNTPEnabled(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    char *uciValue;

    if (strcmp(buf, "true") == 0) uciValue = "1";

    else if (strcmp(buf, "false") == 0) uciValue = "0";

    else return USP_ERR_INTERNAL_ERROR;

    if (SetStringValue("system.ntp.enabled", uciValue) != USP_ERR_OK) return USP_ERR_INTERNAL_ERROR;

    return USP_ERR_OK;
}

int GetLocalTimeZone(dm_req_t *req, char *buf, int len)
{
    char timeZoneVal[64];
    char finalTimeZoneVal[64];

    if (GetStringValue("system.@system[0].timezone", timeZoneVal, sizeof(timeZoneVal)) != USP_ERR_OK)
    {
        return USP_ERR_INTERNAL_ERROR;
    }

    if (strReplace(finalTimeZoneVal, sizeof(finalTimeZoneVal), timeZoneVal, "GMT", "") != 0)
    {
        return USP_ERR_INTERNAL_ERROR;
    }

    snprintf(buf, len, "%s", finalTimeZoneVal);

    return USP_ERR_OK;
}

int SetLocalTimeZone(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    char timeZoneToSet[64];

    snprintf(timeZoneToSet, sizeof(timeZoneToSet), "GMT%s", buf);

    if (SetStringValue("system.@system[0].timezone", timeZoneToSet) != USP_ERR_OK)
    {
        return USP_ERR_INTERNAL_ERROR;
    }

    return USP_ERR_OK;
}

int GetCurrentLocalTime(dm_req_t *req, char *buf, int len)
{
    time_t now;
    struct tm *tm_info;

    time(&now);

    tm_info = localtime(&now);

    if (strftime(buf, len, "%Y-%m-%dT%H:%M:%S", tm_info) == 0)
    {
        return USP_ERR_INTERNAL_ERROR;
    }

    return USP_ERR_OK;
}

int GetNTPStatus(dm_req_t *req, char *buf, int len)
{
    struct timex txc;
    int status;

    memset(&txc, 0, sizeof(struct timex));

    status = adjtimex(&txc);

    if (status != -1 && !(txc.status & STA_UNSYNC))
    {
        snprintf(buf, len, "Synchronized");
    }
    else
    {
        snprintf(buf, len, "Unsynchronized");
    }

    return USP_ERR_OK;
}