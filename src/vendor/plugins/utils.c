#include <string.h>
#include <stdbool.h>

bool endsWith(char *str, char *suffix)
{
    if (!str || !suffix) return false;

    size_t lenStr = strlen(str);
    size_t lenSuffix = strlen(suffix);

    if (lenSuffix > lenStr) return false;

    return strcmp(str + lenStr - lenSuffix, suffix) == 0;
}