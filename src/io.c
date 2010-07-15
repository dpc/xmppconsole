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
static int debug_on = 0;

static void handle_line_fake(char* line) {
	if (line != NULL) {
		if (prog_running) {
			rl_set_prompt(prompt);
			rl_already_prompted = 1;
		}
		return;
	}
	prog_running = 0;

	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	rl_set_prompt(prompt);
}

int io_handle_enter(int x, int y) {
	char* line = NULL;

	line = rl_copy_text(0, rl_end);
	rl_set_prompt("");
	rl_replace_line("", 1);
	rl_redisplay();

	cmd_execute(line);



	if (strcmp(line, "") != 0) {
		add_history(line);
	}
	free(line);

	/* force readline to think that the current line was "eaten" and execute */
	rl_done = 1;
	return 0;
}

void io_debug_set(bool s) {
	debug_on = s;
}

bool io_debug_get() {
	return debug_on;
}

void io_init() {
	FD_ZERO(&fds);
	io_prompt_set(NET_ST_DISCONNECTED);
	rl_callback_handler_install(prompt, handle_line_fake);
	rl_readline_name = prog_name;
	rl_attempted_completion_function = cmd_root_autocompleter;
	if (rl_bind_key(RETURN, io_handle_enter)) {
		io_printfln("failed to bind RETURN");
		abort();
	}
}

void io_nonblock_handle() {
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

void io_deinit() {
	rl_callback_handler_remove();
}

void io_prompt_set(net_status_t st) {
	switch (st) {
		case NET_ST_DISCONNECTED:
			prompt = "_> ";
			break;
		case NET_ST_OFFLINE:
			prompt = "#> ";
			break;
		case NET_ST_ONLINE:
			prompt = "=> ";
			break;
		default:
			prompt = "?> ";
			break;
	}
	rl_set_prompt(prompt);
	rl_redisplay();
}

const char* io_prompt_get() {
	return prompt;
}

/*
void io_set_prompt(net_status_t st) {
	if (prompt == NULL) {
		free(prompt);
	}
	prompt = strdup(str);
	if (prompt == NULL) {
		abort();
	}
	if (prog_running) {
		rl_set_prompt(str);
	} else {
		rl_set_prompt("");
	}
	rl_redisplay();
}*/


void io_vprintfln(const char* const fmt, va_list args) {
	char* saved_line;
	int saved_point;

	saved_point = rl_point;
	saved_line = rl_copy_text(0, rl_end);
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	vprintf(fmt, args);
	printf("\n");
	if (prog_running) {
		rl_set_prompt(prompt);
	}
	rl_replace_line(saved_line, 0);
	rl_point = saved_point;
	rl_redisplay();
	free(saved_line);
}

void io_printfln(const char* const fmt, ...) {
	va_list args;
	va_start(args, fmt);
	io_vprintfln(fmt, args);
	va_end(args);
}

void io_debugln(const char * const fmt, ...) {
	va_list args;
	if (debug_on) {
		va_start(args, fmt);
		io_vprintfln(fmt, args);
		va_end(args);
	}
}

