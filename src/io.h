#ifndef __XC_IO_H__
#define __XC_IO_H__

#include <stdbool.h>

#define DEFAULT_PROMPT "> "

void io_init();
void io_deinit();
void io_switch_debug();
void io_nonblock_handle();
void io_debug_set(bool st);
bool io_debug_get();

void io_set_prompt(const char* const str);
void io_printfln(const char* const fmt, ...);
void io_debugln(const char * const fmt, ...);

#endif
