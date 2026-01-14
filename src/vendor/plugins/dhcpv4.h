#ifndef DHCPV4_H
#define DHCPV4_H

#include "usp_api.h"

int GetLeaseTime(dm_req_t *req, char *buf, int len);
int GetGateway(dm_req_t *req, char *buf, int len);
int SetGateway(dm_req_t *req, char *buf);
int GetSubnetMask(dm_req_t *req, char *buf, int len);
int SetSubnetMask(dm_req_t *req, char *buf);

#endif