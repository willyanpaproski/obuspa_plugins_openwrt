#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "usp_api.h"

int GetHostname(dm_req_t *req, char *buf, int len);
int SetHostname(dm_req_t *req, char *buf);

#endif