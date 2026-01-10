#include "uciHelper.h"
#include "usp_err_codes.h"

#include <uci.h>
#include <string.h>
#include <stdio.h>

int GetStringValue(char *path, char *buf, int len)
{
    struct uci_context *ctx;
    struct uci_ptr ptr;
    char uci_path[64];

    if (path == NULL || buf == NULL || len <= 0) {
        return USP_ERR_INTERNAL_ERROR;
    }

    snprintf(uci_path, sizeof(uci_path), "%s", path);

    ctx = uci_alloc_context();
    if (ctx == NULL) {
        return USP_ERR_INTERNAL_ERROR;
    }

    memset(&ptr, 0, sizeof(ptr));

    if (uci_lookup_ptr(ctx, &ptr, uci_path, true) != UCI_OK ||
        ptr.o == NULL ||
        ptr.o->type != UCI_TYPE_STRING ||
        ptr.o->v.string == NULL ||
        ptr.o->v.string[0] == '\0')
    {
        snprintf(buf, len, "");
    }
    else
    {
        snprintf(buf, len, "%s", ptr.o->v.string);
    }

    uci_free_context(ctx);
    return USP_ERR_OK;
}
