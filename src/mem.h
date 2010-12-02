#ifndef __XC_MEM_H__
#define __XC_MEM_H__

#include <apr_pools.h>

void mem_init();
void mem_deinit();

extern apr_pool_t* mem_main_pool;

#endif
