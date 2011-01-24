#ifndef __XC_UTIL_H__
#define __XC_UTIL_H__

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define OOM_CHECK(c) (oom_check_impl((__FILE__), (__LINE__), (c)))

#define ALLOC(p)  (p = OOM_CHECK(malloc(sizeof(*p))));
#define BZERO(p) (bzero(p, sizeof(*p)));
#define NEW(p) ALLOC(p); BZERO(p);
#define FREE(p) free(p);

void *oom_check_impl(const char* file, int line, void* x);

#endif
