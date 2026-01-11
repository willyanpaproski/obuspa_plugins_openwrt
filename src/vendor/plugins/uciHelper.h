#ifndef UCI_HELPER
#define UCI_HELPER

int GetStringValue(char *path, char *buf, int len);
int GetListValues(const char *path, char values[][64], int max_values, int value_len, int *out_count);
int SetStringValue(const char *path, const char *value);
int SetListValues(const char *path, char values[][64], int count);

#endif