#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "usp_api.h"

int GetDeviceInfoParams(int group_id, kv_vector_t *params);
int SetDeviceInfoParams(int group_id, kv_vector_t *params, unsigned *types, int *failure_index);
int GetHostname(dm_req_t *req, char *buf, int len);
int SetHostname(dm_req_t *req, char *buf);
int GetTotalMemory(dm_req_t *req, char *buf, int len);
int GetFreeMemory(dm_req_t *req, char *buf, int len);
int GetProccessAmount(dm_req_t *req, char *buf, int len);
int GetCPULoad(dm_req_t *req, char *buf, int len);

#endif