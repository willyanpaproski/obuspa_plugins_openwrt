#ifndef DHCPV4_H
#define DHCPV4_H

#include "usp_api.h"

int GetLeaseTime(dm_req_t *req, char *buf, int len);
int SetLeaseTime(dm_req_t *req, char *buf);
int GetGateway(dm_req_t *req, char *buf, int len);
int SetGateway(dm_req_t *req, char *buf);
int GetSubnetMask(dm_req_t *req, char *buf, int len);
int SetSubnetMask(dm_req_t *req, char *buf);
int GetMinAddress(dm_req_t *req, char *buf, int len);
int GetMaxAddress(dm_req_t *req, char *buf, int len);
int GetDnsServers(dm_req_t *req, char *buf, int len);
int SetDnsServers(dm_req_t *req, char *buf);
int ValidateAddPool(dm_req_t *req);
int ValidateRemovePool(dm_req_t *req);

#endif