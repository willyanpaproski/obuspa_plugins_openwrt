#ifndef NTP_H
#define NTP_H

#include "usp_api.h"

int SetTimeParams(int group_id, kv_vector_t *params, unsigned *types, int *failure_index);
int GetNTPServer1(dm_req_t *req, char *buf, int len);
int SetNTPServer1(dm_req_t *req, char *buf);
int GetNTPServer2(dm_req_t *req, char *buf, int len);
int SetNTPServer2(dm_req_t *req, char *buf);
int GetNTPEnabled(dm_req_t *req, char *buf, int len);
int SetNTPEnabled(dm_req_t *req, char *buf);

#endif