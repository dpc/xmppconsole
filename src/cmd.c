#include "cmd.h"
#include "cmds.h"
#include "io.h"
#include <string.h>
#include <stdlib.h>

struct cmd_t {
	const char * const cmd;
	void (*handle)(const char * const);
};

static struct cmd_t cmds[] = {
	{ "connect", cmd_connect },
	{ "disconnect", cmd_disconnect },
	{ "quit", cmd_quit}
};

static void cmd_slash(const char * const str) {
	int i;
	char *s;
	char* cmd;
	s = strdup(str);
	cmd = strtok(s, " \t");
	for (i = 0; i < sizeof(cmds) / sizeof(struct cmd_t); ++i) {
		if (strcmp(cmds[i].cmd, cmd) == 0) {
			cmds[i].handle(str);
			goto ret;
		}
	}
	printf_async("Unknown command: %s\n", cmd);
ret:
	free(s);
}

static void cmd_at(const char * const str) {
}

static void cmd_msg(const char * const str) {
}

void interprete(const char * const str) {
	if (strcmp(str, "") == 0) {
		return;
	}

	switch (str[0]) {
		case '/':
			cmd_slash(str + 1);
			break;
		case '@':
			cmd_at(str + 1);
			break;
		default:
			cmd_msg(str);
	}
}

