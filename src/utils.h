#ifndef UTILS_H_
#define UTILS_H_

#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int starts_with(const char* pre, const char* str);
int starts_with_case_insensitive(const char* pre, const char* str);
int str_equal(const char* str1, const char* str2);
int str_in_array(const char** array, int array_length, const char* str);

#endif