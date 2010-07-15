#ifndef __XC_UTIL_H__
#define __XC_UTIL_H__

#include <stddef.h>

#define safe_malloc(c) (safe_malloc_impl((__FILE__), (__LINE__), (c)))

void *safe_malloc_impl(const char* file, int line, size_t size);

char* safe_strdup(const char* str);
char* safe_strndup(const char* str, size_t n);

#endif
