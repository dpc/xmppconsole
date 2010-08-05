#include "net.h"
#include "io.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include "msg.h"

#include "cmds.h"


#define ARGGET(x) if (!tokens) { \
	io_error("insufficient number of arguments when looking for `%s'", #x); \
	goto ret; \
	} \
	(x) = tokens->name; tokens=tokens->next;

#define ARGEND if (tokens) { \
	io_error("too many arguments: `%s'", tokens->name); \
	goto ret; \
	}

void cmd_connect_h(const struct cmd_tokenized_node* tokens) {
	char* jid;
	char* pass;

	ARGGET(jid);
	ARGEND;

	pass = io_getpass();

	net_connect(jid, pass);
	free(pass);
ret:
	return;
}

void cmd_help_h(const struct cmd_tokenized_node* tokens) {

	io_printfln("Basic usage:");
	io_printfln("/connect <jid>");
	io_printfln("  -- to connect");
	io_printfln("/select <jid>");
	io_printfln("  -- to select recipient");
	io_printfln("Use TAB key after slash sign to auto-complete and discover commands.");
	io_printfln("Type text followed by ENTER to send a message.");
}


void cmd_disconnect_h(const struct cmd_tokenized_node* tokens) {
	ARGEND;

	net_disconnect();
ret:
	return;
}

void cmd_quit_h(const struct cmd_tokenized_node* tokens) {

	ARGEND;

	prog_running = 0;
ret:
	return;
}

void cmd_debug_h(const struct cmd_tokenized_node* tokens) {

	ARGEND;

	io_printfln("Debug mode: %s", (io_debug_get()) ? "on" : "off");
ret:
	return;
}

void cmd_debug_switch_h(const struct cmd_tokenized_node* tokens) {

	ARGEND;

	io_debug_get(!io_debug_get());

ret:
	return;
}

void cmd_debug_on_h(const struct cmd_tokenized_node* tokens) {

	ARGEND;

	io_debug_set(true);

ret:
	return;
}

void cmd_debug_off_h(const struct cmd_tokenized_node* tokens) {

	ARGEND;

	io_debug_set(false);

ret:
	return;
}

void cmd_unread_h(const struct cmd_tokenized_node* tokens) {
	msg_queue_t q;

	ARGEND;

	q = NULL;
	io_notification("Unread list:");
	while ((q = msg_queue_iterate(q))) {
		if (!msg_queue_empty(q)) {
			io_printfln("%s", msg_queue_jid(q));
		}
	}

ret:
	return;
}

char* cmd_select_complete_h(int i, const struct cmd_tokenized_node* tokens) {
	static msg_queue_t q;
	msg_queue_t tq;

	if (i == 0) {
		q = msg_queue_iterate(NULL);
	}

	while (q) {

		tq = q;
		q = msg_queue_iterate(q);

		if (!msg_queue_empty(tq)) {
			return OOM_CHECK(strdup(msg_queue_jid(tq)));
		}
	}

	return NULL;
}


void cmd_select_h(const struct cmd_tokenized_node* tokens) {
	char* alias;

	ARGGET(alias);
	ARGEND;

	msg_active_queue_set(msg_queue_get(alias));
ret:
	return;
}

void cmds_default(const char* str) {
	net_send(str);
}



struct cmd_descriptor cmd_connect = { "/connect", cmd_connect_h, 0, 0};
struct cmd_descriptor cmd_disconnect= { "/disconnect", cmd_disconnect_h, 0, 0};
struct cmd_descriptor cmd_help = { "/help", cmd_help_h, 0, 0};

struct cmd_descriptor cmd_debug_on = { "on", cmd_debug_on_h, 0, 0};
struct cmd_descriptor cmd_debug_off = { "off", cmd_debug_off_h, 0, 0};
struct cmd_descriptor cmd_debug_switch = { "switch", cmd_debug_switch_h, 0, 0};

struct cmd_descriptor cmd_unread= { "/unread", cmd_unread_h, 0, 0};
struct cmd_descriptor cmd_select = { "/select", cmd_select_h, 0, cmd_select_complete_h};
struct cmd_descriptor cmd_quit = { "/quit", cmd_quit_h, 0, 0};

cmd_descriptor_t cmds_debug[] = {
	&cmd_debug_on,
	&cmd_debug_off,
	&cmd_debug_switch,
	NULL
};
struct cmd_descriptor cmd_debug = { "/debug", cmd_debug_h, cmds_debug, 0};

cmd_descriptor_t cmds_root[] = {
	&cmd_debug,
	&cmd_connect,
	&cmd_disconnect,
	&cmd_help,
	&cmd_select,
	&cmd_unread,
	&cmd_quit,
	NULL
};
