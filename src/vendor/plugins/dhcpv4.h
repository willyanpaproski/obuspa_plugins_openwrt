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
int SetMinAddress(dm_req_t *req, char *buf);
int GetMaxAddress(dm_req_t *req, char *buf, int len);
int SetMaxAddress(dm_req_t *req, char *buf);
int GetDnsServers(dm_req_t *req, char *buf, int len);
int SetDnsServers(dm_req_t *req, char *buf);
int GetDomainName(dm_req_t *req, char *buf, int len);
int SetDomainName(dm_req_t *req, char *buf);
int GetPoolEnabled(dm_req_t *req, char *buf, int len);
int SetPoolEnabled(dm_req_t *req, char *buf);
int ValidateAddPool(dm_req_t *req);
int ValidateRemovePool(dm_req_t *req);

#endif