#include "net.h"
#include "io.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>

#include "cmds.h"

void cmd_connect_h(const cmd_tokenized_node_t*);
void cmd_disconnect_h(const cmd_tokenized_node_t*);
void cmd_debug_h(const cmd_tokenized_node_t*);
void cmd_debug_on_h(const cmd_tokenized_node_t*);
void cmd_debug_off_h(const cmd_tokenized_node_t*);
void cmd_debug_switch_h(const cmd_tokenized_node_t*);
void cmd_quit_h(const cmd_tokenized_node_t*);
void cmd_select_h(const cmd_tokenized_node_t*);


cmd_descriptor_t cmd_connect = { "/connect", cmd_connect_h, 0, 0};
cmd_descriptor_t cmd_disconnect= { "/disconnect", cmd_disconnect_h, 0, 0};

cmd_descriptor_t cmd_debug_on = { "on", cmd_debug_on_h, 0, 0};
cmd_descriptor_t cmd_debug_off = { "off", cmd_debug_off_h, 0, 0};
cmd_descriptor_t cmd_debug_switch = { "switch", cmd_debug_switch_h, 0, 0};
cmd_descriptor_t* cmds_debug[] = {
	&cmd_debug_on,
	&cmd_debug_off,
	&cmd_debug_switch,
	NULL
};
cmd_descriptor_t cmd_debug = { "/debug", cmd_debug_h, cmds_debug, 0};
cmd_descriptor_t cmd_select = { "/select", cmd_select_h, 0, 0};
cmd_descriptor_t cmd_quit = { "/quit", cmd_quit_h, 0, 0};

cmd_descriptor_t* cmds_root[] = {
	&cmd_connect,
	&cmd_disconnect,
	&cmd_quit,
	&cmd_select,
	&cmd_debug,
	NULL
};

#define ARGGET(x) if (!tokens) { \
	io_printfln("insufficient number of arguments when looking for `%s'", #x); \
	goto ret; \
	} \
	(x) = tokens->name; tokens=tokens->next;

#define ARGEND if (tokens) { \
	io_printfln("too many arguments: `%s'", tokens->name); \
	goto ret; \
	}

void cmd_connect_h(const cmd_tokenized_node_t* tokens) {
	char* jid;
	char* pass;

	ARGGET(jid);
	ARGGET(pass);
	ARGEND;

	net_connect(jid, pass);
ret:
	return;
}

void cmd_disconnect_h(const cmd_tokenized_node_t* tokens) {
	ARGEND;

	net_disconnect();
ret:
	return;
}

void cmd_quit_h(const cmd_tokenized_node_t* tokens) {

	ARGEND;

	prog_running = 0;
ret:
	return;
}

void cmd_debug_h(const cmd_tokenized_node_t* tokens) {

	ARGEND;

	io_printfln("Debug mode: %s", (io_debug_get()) ? "on" : "off");
ret:
	return;
}

void cmd_debug_switch_h(const cmd_tokenized_node_t* tokens) {

	ARGEND;

	io_debug_get(!io_debug_get());

ret:
	return;
}

void cmd_debug_on_h(const cmd_tokenized_node_t* tokens) {

	ARGEND;

	io_debug_set(true);

ret:
	return;
}

void cmd_debug_off_h(const cmd_tokenized_node_t* tokens) {

	ARGEND;

	io_debug_set(false);

ret:
	return;
}

void cmd_select_h(const cmd_tokenized_node_t* tokens) {
	char* alias;

	ARGGET(alias);
	ARGEND;

	io_debug_set(false);

ret:
	return;
}
void cmds_default(const char* str) {
	net_set_current_recipent(str);
}
