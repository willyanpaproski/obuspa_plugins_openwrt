#include "deviceInfo.h"
#include "usp_err_codes.h"
#include "uciHelper.h"
#include "utils.h"

#include <string.h>
#include <stdio.h>
#include <sys/sysinfo.h>

int GetDeviceInfoParams(int group_id, kv_vector_t *params)
{
    char buf[256];

    for (int i = 0; i < params->num_entries; i++)
    {
        kv_pair_t *kv = &params->vector[i];

        if (strcmp(kv->key, "Device.DeviceInfo.X_IXC_Hostname") == 0)
        {
            GetHostname(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DeviceInfo.MemoryStatus.Free") == 0)
        {
            GetFreeMemory(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DeviceInfo.MemoryStatus.Total") == 0)
        {
            GetTotalMemory(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DeviceInfo.ProcessStatus.CPUUsage") == 0)
        {
            GetCPULoad(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }

        if (strcmp(kv->key, "Device.DeviceInfo.ProcessStatus.ProcessNumberOfEntries") == 0)
        {
            GetProccessAmount(NULL, buf, sizeof(buf));
            replaceKVValue(kv, buf);
        }
    }

    return USP_ERR_OK;
}

int SetDeviceInfoParams(int group_id, kv_vector_t *params, unsigned *types, int *failure_index)
{
    bool needs_restart = false;
    int err = USP_ERR_OK;

    for (int i = 0; i < params->num_entries; i++)
    {
        char *key = params->vector[i].key;
        char *val = params->vector[i].value;

        if (strcmp(key, "Device.DeviceInfo.X_IXC_Hostname") == 0)
        {
            err = SetHostname(NULL, val);
        }

        needs_restart = true;
    }

    if (needs_restart)
    {
        system("/etc/init.d/system restart");
    }

    return err;
}

int GetHostname(dm_req_t *req, char *buf, int len)
{
    (void)req;

    return GetStringValue("system.@system[0].hostname", buf, len);
}

int SetHostname(dm_req_t *req, char *buf)
{
    if (buf == NULL) return USP_ERR_INTERNAL_ERROR;

    int ret = SetStringValue("system.@system[0].hostname", buf);

    return ret;
}

int GetTotalMemory(dm_req_t *req, char *buf, int len)
{
    struct sysinfo si;

    if (sysinfo(&si) != 0) return USP_ERR_INTERNAL_ERROR;

    unsigned long total_mem = si.totalram * si.mem_unit;

    snprintf(buf, len, "%lu", total_mem);

    return USP_ERR_OK;
}

int GetFreeMemory(dm_req_t *req, char *buf, int len)
{
    struct sysinfo si;

    if (sysinfo(&si) != 0) return USP_ERR_INTERNAL_ERROR;

    unsigned long free_mem = si.freeram * si.mem_unit;

    snprintf(buf, len, "%lu", free_mem);

    return USP_ERR_OK;
}

int GetProccessAmount(dm_req_t *req, char *buf, int len)
{
    struct sysinfo si;

    if (sysinfo(&si) != 0) return USP_ERR_INTERNAL_ERROR;

    snprintf(buf, len, "%u", si.procs);

    return USP_ERR_OK;
}

int GetCPULoad(dm_req_t *req, char *buf, int len)
{
    struct sysinfo si;

    if (sysinfo(&si) != 0) return USP_ERR_INTERNAL_ERROR;

    float load = si.loads[0] / 65536.0f;

    snprintf(buf, len, "%.2f", load);

    return USP_ERR_OK;
}