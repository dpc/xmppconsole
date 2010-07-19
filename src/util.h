#ifndef __XC_UTIL_H__
#define __XC_UTIL_H__

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define safe_malloc(c) (safe_malloc_impl((__FILE__), (__LINE__), (c)))

#define ALLOC(p)  (p = safe_malloc(sizeof(*p)));
#define BZERO(p) (bzero(p, sizeof(*p)));
#define NEW(p) ALLOC(p); BZERO(p);
#define FREE(p) free(p);

void *safe_malloc_impl(const char* file, int line, size_t size);

char* safe_strdup(const char* str);
char* safe_strndup(const char* str, size_t n);


#endif
