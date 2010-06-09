#include "io.h"
#include "cmd.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#include "main.h"

static char* prompt = NULL;
static fd_set fds;

static void handle_line(char* line) {
	if (line == NULL) {
		/* eof */
		prog_running = 0;
		goto ret;
	}

	interprete(line);
	if (strcmp(line, "") != 0) {
		add_history(line);
	}
ret:
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	rl_set_prompt(prompt);
	return;
}

void init_io() {
	FD_ZERO(&fds);
	set_prompt(DEFAULT_PROMPT);
	rl_callback_handler_install(prompt, handle_line);
}

void nonblock_handle_io() {
	int count;
	struct timeval t;

	t.tv_sec = 0;
	t.tv_usec = 0;

	FD_SET(STDIN_FILENO, &fds);
	count = select(FD_SETSIZE, &fds, NULL, NULL, &t);
	if (count < 0) {
		exit(1);
	} else if (count == 0) {
		return;
	}
	rl_callback_read_char();
}

void deinit_io() {
	rl_callback_handler_remove();
}

void set_prompt(const char* const str) {
	if (prompt == NULL) {
		free(prompt);
	}
	prompt = strdup(str);
	if (prompt == NULL) {
		abort();
	}
}


void printf_async(const char* const fmt, ...) {
	va_list args;
	char* saved_line;
	int saved_point;

	saved_point = rl_point;
	saved_line = rl_copy_text(0, rl_end);
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	if (prog_running) {
		rl_set_prompt(prompt);
	}
	rl_replace_line(saved_line, 0);
	rl_point = saved_point;
	rl_redisplay();
	free(saved_line);
}


