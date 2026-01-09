#include "device.h"
#include "usp_err_codes.h"
#include <string.h>

int GetTeste(dm_req_t *req, char *buf, int len)
{
    (void)req;

    strncpy(buf, "Teste", len);
    return USP_ERR_OK;
}