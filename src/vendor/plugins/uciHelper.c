#include "uciHelper.h"
#include "usp_err_codes.h"
#include <uci.h>
#include <string.h>
#include <stdio.h>

int GetStringValue(char *path, char *fallback, char *buf, int len)
{
    struct uci_context *ctx = NULL;
    struct uci_ptr ptr;
    int res = USP_ERR_OK;

    if (path == NULL || buf == NULL || len <= 0) return USP_ERR_INTERNAL_ERROR;

    ctx = uci_alloc_context();
    if (ctx == NULL) return USP_ERR_INTERNAL_ERROR;

    char path_copy[128];
    snprintf(path_copy, sizeof(path_copy), "%s", path);

    if (uci_lookup_ptr(ctx, &ptr, path_copy, true) != UCI_OK || 
        ptr.o == NULL || 
        ptr.o->type != UCI_TYPE_STRING ||
        ptr.o->v.string == NULL) 
    {
        snprintf(buf, len, "%s", fallback);
    } 
    else 
    {
        snprintf(buf, len, "%s", ptr.o->v.string);
    }

    uci_free_context(ctx);
    return res;
}