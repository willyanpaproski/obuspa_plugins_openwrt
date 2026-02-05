#ifndef NTP_H
#define NTP_H

#include "usp_api.h"

int SetTimeParams(dm_req_t *req, char *path, kv_vector_t *params);
int GetNTPServer1(dm_req_t *req, char *buf, int len);
int SetNTPServer1(dm_req_t *req, char *buf);
int GetNTPServer2(dm_req_t *req, char *buf, int len);
int SetNTPServer2(dm_req_t *req, char *buf);
int GetNTPEnabled(dm_req_t *req, char *buf, int len);
int SetNTPEnabled(dm_req_t *req, char *buf);
int SetNTPEnabledValidator(dm_req_t *req, char *buf);

#endif