#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *safe_malloc_impl(const char* file, int line, size_t size) {
	void* ret;

	ret = malloc(size);
	if (!ret) {
		io_deinit();
		fprintf(stderr, "Out of memory: %s, %d", file, line);
		abort();
	}

	return ret;
}

char* safe_strdup(const char* str) {
	char* s;
	s = strdup(str);
	if (!s) {
		abort();
	}
	return s;
}

char* safe_strndup(const char* str, size_t n) {
	char* s;
	s = strndup(str, n);
	if (!s) {
		abort();
	}
	return s;
}
