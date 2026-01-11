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
    char path_copy[256];

    if (!path || !values || !out_count || max_values <= 0 || value_len <= 0) {
        return USP_ERR_INTERNAL_ERROR;
    }

    *out_count = 0;

    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    ctx = uci_alloc_context();
    if (!ctx) {
        return USP_ERR_INTERNAL_ERROR;
    }

    memset(&ptr, 0, sizeof(ptr));

    if (uci_lookup_ptr(ctx, &ptr, path_copy, true) != UCI_OK ||
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

int SetStringValue(const char *path, const char *value)
{
    struct uci_context *ctx;
    struct uci_ptr ptr;
    char path_copy[256];
    int ret = USP_ERR_OK;

    if (!path || !value) return USP_ERR_INTERNAL_ERROR;

    ctx = uci_alloc_context();
    if (!ctx) return USP_ERR_INTERNAL_ERROR;

    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    if (uci_lookup_ptr(ctx, &ptr, path_copy, true) != UCI_OK) {
        ret = USP_ERR_INTERNAL_ERROR;
        goto cleanup;
    }

    ptr.value = value;

    if (uci_set(ctx, &ptr) != UCI_OK) {
        ret = USP_ERR_INTERNAL_ERROR;
        goto cleanup;
    }

    uci_commit(ctx, &ptr.p, false);

cleanup:
    uci_free_context(ctx);
    return ret;
}

int SetListValues(const char *path, char values[][64], int count)
{
    struct uci_context *ctx;
    struct uci_ptr ptr;
    char path_copy[256];
    int ret = USP_ERR_OK;

    ctx = uci_alloc_context();
    if (!ctx) return USP_ERR_INTERNAL_ERROR;

    strncpy(path_copy, path, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    if (uci_lookup_ptr(ctx, &ptr, path_copy, true) != UCI_OK) {
        ret = USP_ERR_INTERNAL_ERROR;
        goto cleanup;
    }

    uci_delete(ctx, &ptr);

    for (int i = 0; i < count; i++) {
        memset(&ptr, 0, sizeof(ptr));
        strncpy(path_copy, path, sizeof(path_copy) - 1); 
        uci_lookup_ptr(ctx, &ptr, path_copy, true);
        
        ptr.value = values[i];
        uci_add_list(ctx, &ptr);
    }

    uci_commit(ctx, &ptr.p, false);

cleanup:
    uci_free_context(ctx);
    return ret;
}