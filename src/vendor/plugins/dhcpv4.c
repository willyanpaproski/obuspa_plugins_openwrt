#include "dhcpv4.h"
#include "usp_err_codes.h"
#include "uciHelper.h"

#include <string.h>
#include <stdio.h>

int GetLeaseTime(dm_req_t *req, char *buf, int len)
{
    (void)req;
    
    return GetStringValue("dhcp.lan.leasetime", "12h", buf, len);
}