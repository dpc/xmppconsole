#include "io.h"
#include "cmd.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/select.h>

#include "util.h"
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

	rl_set_prompt(prompt);
	rl_redisplay();

	/* force readline to think that the current line was "eaten" and executed */
	rl_done = 1;
	return 0;
}

static void install_line_handler() {
	if (rl_bind_key(RETURN, io_handle_enter)) {
		io_printfln("failed to bind RETURN");
		abort();
	}
	rl_callback_handler_install(prompt, handle_line_fake);
}

static void remove_line_handler() {
	rl_unbind_key(RETURN);
	rl_callback_handler_remove();
}

void io_debug_set(bool s) {
	debug_on = s;
}

bool io_debug_get() {
	return debug_on;
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

void io_prompt_set(net_status_t st) {
	switch (st) {
		case NET_ST_DISCONNECTED:
			prompt = "__> ";
			break;
		case NET_ST_OFFLINE:
			prompt = "=_> ";
			break;
		case NET_ST_ONLINE:
			prompt = "==> ";
			break;
		default:
			prompt = "=?> ";
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

typedef void (*print_func)(void*);

static void io_async_print(print_func func, void* data) {
	char* saved_line;
	int saved_point;

	saved_point = rl_point;
	saved_line = rl_copy_text(0, rl_end);

	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	(*func)(data);
	if (prog_running) {
		rl_set_prompt(prompt);
		rl_replace_line(saved_line, 0);
		rl_point = saved_point;
		rl_redisplay();
	} else {
		/* FIXME: This leaves prompt on /quit. Fix it. */
		rl_redisplay();
	}
	free(saved_line);
}

struct io_async_func_data {
	const char* fmt;
	va_list args;
};

/*
void io_vprintfln(const char* const fmt, va_list args) {

	struct io_async_func_data* d = (struct io_async_func_data*)data;
	vprintf(d->fmt, d->args);
}*/

static void io_notification_func(void* data) {
	struct io_async_func_data* d = (struct io_async_func_data*)data;

	printf("*** ");
	vprintf(d->fmt, d->args);
	printf("\n");
}

static void io_error_func(void* data) {
	struct io_async_func_data* d = (struct io_async_func_data*)data;

	printf("!!! ");
	vprintf(d->fmt, d->args);
	printf("\n");
}

static void io_print_func(void* data) {
	struct io_async_func_data* d = (struct io_async_func_data*)data;

	vprintf(d->fmt, d->args);
	printf("\n");
}

void io_printfln(const char* const fmt, ...) {
	struct io_async_func_data d;
	d.fmt = fmt;

	va_start(d.args, fmt);
	io_async_print(io_print_func, &d);
	va_end(d.args);
}

void io_debug(const char * const fmt, ...) {
	struct io_async_func_data d;
	d.fmt = fmt;

	if (debug_on) {
		va_start(d.args, fmt);
		io_async_print(io_print_func, &d);
		va_end(d.args);
	}
}

void io_error(const char * const fmt, ...) {
	struct io_async_func_data d;
	d.fmt = fmt;
	
	va_start(d.args, fmt);
	io_async_print(io_error_func, &d);
	va_end(d.args);
}

void io_notification(const char * const fmt, ...) {
	struct io_async_func_data d;
	d.fmt = fmt;
	
	va_start(d.args, fmt);
	io_async_print(io_notification_func, &d);
	va_end(d.args);
}

void io_message(const char* jid, const char* msg) {
	char* s;
	char* node;
   
	s = OOM_CHECK(strdup(jid));
	node = strtok(s, "@");

	io_printfln("%s: %s", node, msg);
	free(s);
}

void io_getpass_func(void* d) {
	char** ret = (char**)d;
	char* pass;

	pass = getpass("password: ");
	*ret = strdup(pass);
	//rl_already_prompted = true;
}

char* io_getpass() {
	char *pass;
	remove_line_handler();
	io_async_print(io_getpass_func, &pass);
	install_line_handler();
	return pass;
}

void io_init() {
	FD_ZERO(&fds);
	io_prompt_set(NET_ST_DISCONNECTED);
	install_line_handler();
	rl_readline_name = prog_name;
	rl_attempted_completion_function = cmd_root_autocompleter;

}

void io_deinit() {
	remove_line_handler();
}


