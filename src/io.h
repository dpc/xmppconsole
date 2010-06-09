#define DEFAULT_PROMPT "> "

void io_init();
void io_deinit();
void io_switch_debug();
void io_nonblock_handle();
void io_debug(const char * const str);
void io_set_prompt(const char* const str);
void printf_async(const char* const fmt, ...);
