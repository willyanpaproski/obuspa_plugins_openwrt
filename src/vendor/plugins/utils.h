#ifndef UTILS_H
#define UTILS_H

bool endsWith(char *str, char *suffix);
int strReplace(char *dest, size_t dest_size, const char *src, const char *old, const char *new);
int validateUspBoolean(char *value);
void replaceKVValue(kv_pair_t *kv, char *newValue);
bool isValidIPv4Address(const char *ip);
bool isValidIPv6Address(const char *ip);

#endif