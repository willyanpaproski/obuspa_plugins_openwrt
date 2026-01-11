#ifndef NTP_H
#define NTP_H

#include "usp_api.h"

int GetNTPServer1(dm_req_t *req, char *buf, int len);
int GetNTPServer2(dm_req_t *req, char *buf, int len);

#endif