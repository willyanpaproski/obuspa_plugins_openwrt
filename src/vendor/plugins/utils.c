#include <string.h>
#include <stdbool.h>
#include "usp_err_codes.h"
#include "usp_api.h"
#include "usp_mem.h"

bool endsWith(char *str, char *suffix)
{
    if (!str || !suffix) return false;

    size_t lenStr = strlen(str);
    size_t lenSuffix = strlen(suffix);

    if (lenSuffix > lenStr) return false;

    return strcmp(str + lenStr - lenSuffix, suffix) == 0;
}

int strReplace(
    char *dest, size_t dest_size,
    const char *src,
    const char *old,
    const char *new
) {
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t dest_len = 0;

    while (*src) {
        if (strncmp(src, old, old_len) == 0) {
            if (dest_len + new_len >= dest_size)
                return -1;

            memcpy(dest + dest_len, new, new_len);
            dest_len += new_len;
            src += old_len;
        } else {
            if (dest_len + 1 >= dest_size)
                return -1;

            dest[dest_len++] = *src++;
        }
    }

    dest[dest_len] = '\0';
    return 0;
}

int validateUspBoolean(char *value)
{
    if (value == NULL) return USP_ERR_INVALID_VALUE;

    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0)
    {
        return USP_ERR_OK;
    }

    return USP_ERR_INVALID_VALUE;
}

void replaceKVValue(kv_pair_t *kv, char *newValue)
{
    if (kv->value != NULL)
    {
        USP_SAFE_FREE(kv->value);
    }

    kv->value = USP_STRDUP(newValue);
}