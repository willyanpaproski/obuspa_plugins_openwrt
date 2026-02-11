#ifndef DHCPV4_H
#define DHCPV4_H

#include "usp_api.h"

int GetDHCPv4Params(int group_id, kv_vector_t *params);
int SetDHCPv4Params(int group_id, kv_vector_t *params, unsigned *types, int *failure_index);
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
bool isDnsMasqRunning();
int GetDHCPv4Status(dm_req_t *req, char *buf, int len);
int ValidateAddPool(dm_req_t *req);
int ValidateRemovePool(dm_req_t *req);

#endif