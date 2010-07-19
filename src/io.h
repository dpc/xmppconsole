#ifndef __XC_IO_H__
#define __XC_IO_H__

#include <stdbool.h>
#include "net.h"

void io_init();
void io_deinit();
void io_switch_debug();
void io_nonblock_handle();
void io_debug_set(bool st);
bool io_debug_get();

void io_prompt_set(net_status_t status);
const char* io_prompt_get();

void io_notification(const char* const fmt, ...);
void io_error(const char* const fmt, ...);
void io_message(const char* jid, const char* msg);
void io_debug(const char * const fmt, ...);
void io_printfln(const char* const fmt, ...);

#endif
