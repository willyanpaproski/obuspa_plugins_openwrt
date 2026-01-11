#include "uciHelper.h"
#include "usp_err_codes.h"

#include <uci.h>
#include <string.h>
#include <stdio.h>

int GetStringValue(char *path, char *buf, int len)
{
    struct uci_context *ctx;
    struct uci_ptr ptr;

    if (path == NULL || buf == NULL || len <= 0) {
        return USP_ERR_INTERNAL_ERROR;
    }

    ctx = uci_alloc_context();
    if (ctx == NULL) {
        return USP_ERR_INTERNAL_ERROR;
    }

    memset(&ptr, 0, sizeof(ptr));

    if (uci_lookup_ptr(ctx, &ptr, path, true) != UCI_OK ||
        ptr.o == NULL ||
        ptr.o->type != UCI_TYPE_STRING ||
        ptr.o->v.string == NULL)
    {
        buf[0] = '\0';
    }
    else
    {
        snprintf(buf, len, "%s", ptr.o->v.string);
    }

    uci_free_context(ctx);
    return USP_ERR_OK;
}

int GetListValues(const char *path,
                  char values[][64],
                  int max_values,
                  int value_len,
                  int *out_count)
{
    struct uci_context *ctx;
    struct uci_ptr ptr;
    struct uci_element *e;
    int count = 0;

    if (!path || !values || !out_count || max_values <= 0 || value_len <= 0) {
        return USP_ERR_INTERNAL_ERROR;
    }

    *out_count = 0;

    ctx = uci_alloc_context();
    if (!ctx) {
        return USP_ERR_INTERNAL_ERROR;
    }

    memset(&ptr, 0, sizeof(ptr));

    if (uci_lookup_ptr(ctx, &ptr, (char *)path, true) != UCI_OK ||
        ptr.o == NULL ||
        ptr.o->type != UCI_TYPE_LIST)
    {
        uci_free_context(ctx);
        return USP_ERR_OK;
    }

    uci_foreach_element(&ptr.o->v.list, e)
    {
        if (count >= max_values) {
            break;
        }

        snprintf(values[count], value_len, "%s", e->name);
        count++;
    }

    *out_count = count;

    uci_free_context(ctx);
    return USP_ERR_OK;
}
