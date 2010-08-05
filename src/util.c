#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *oom_check_impl(const char* file, int line, void* x) {
	if (!x) {
		fprintf(stderr, "Out of memory: %s, %d", file, line);
		abort();
	}

	return x;
}
