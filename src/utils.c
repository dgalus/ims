#include "utils.h"

int starts_with(const char* pre, const char* str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

int starts_with_case_insensitive(const char* pre, const char* str)
{
    char* m_pre = strdup(pre);
    char* m_str = strdup(str);

    int i;
    for (i = 0; m_pre[i]; i++)
        m_pre[i] = tolower(m_pre[i]);

    for (i = 0; m_str[i]; i++)
        m_str[i] = tolower(m_str[i]);

    int res = strncmp(m_pre, m_str, strlen(m_pre)) == 0;

    free((void*)m_pre);
    free((void*)m_str);
    return res;
}

int str_equal(const char* str1, const char* str2)
{
    return strcmp(str1, str2) == 0;
}

int str_in_array(const char** array, int array_length, const char* str)
{
    int i;
    for (i = 0; i < array_length; i++)
        if (str_equal(array[i], str))
            return 1;
    return 0;
}