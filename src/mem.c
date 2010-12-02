#include "mem.h"
#include <apr.h>
#include <apr_errno.h>
#include <apr_pools.h>

#include <stdlib.h>
#include <stdio.h>

apr_pool_t* mem_main_pool = NULL;

void mem_init() {
	apr_status_t r;
	char buf[100];

	r = apr_pool_initialize();

	if (r != APR_SUCCESS) {
		fprintf(stderr,
				"Couldn't init memory pool: `%s'\n",
				apr_strerror(r, buf, sizeof(buf))
			   );
		abort();
	}

	r = apr_pool_create(&mem_main_pool, NULL);
	
	if(r != APR_SUCCESS) {
		fprintf(stderr,
				"Couldn't init memory pool: `%s'\n",
				apr_strerror(r, buf, sizeof(buf))
			   );
		abort();
	}
}

void mem_deinit() {
	apr_pool_destroy(mem_main_pool);
	apr_pool_terminate();
}
