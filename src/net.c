#include "net.h"
#include <strophe.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "io.h"
#include "main.h"

static xmpp_ctx_t *ctx = NULL;
static xmpp_conn_t *conn = NULL;
static xmpp_log_t *log;

int version_handler(
		xmpp_conn_t * const conn, xmpp_stanza_t * const stanza,
		void * const userdata
		)
{
	xmpp_stanza_t *reply, *query, *name, *version, *text;
	char *ns;
	xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;
	printf_async(
			"Received version request from %s\n",
			xmpp_stanza_get_attribute(stanza, "from")
			);

	reply = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(reply, "iq");
	xmpp_stanza_set_type(reply, "result");
	xmpp_stanza_set_id(reply, xmpp_stanza_get_id(stanza));
	xmpp_stanza_set_attribute(
			reply, "to", xmpp_stanza_get_attribute(stanza, "from")
			);

	query = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(query, "query");
	ns = xmpp_stanza_get_ns(xmpp_stanza_get_children(stanza));
	if (ns) {
		xmpp_stanza_set_ns(query, ns);
	}

	name = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(name, "name");
	xmpp_stanza_add_child(query, name);

	text = xmpp_stanza_new(ctx);
	xmpp_stanza_set_text(text, prog_name);
	xmpp_stanza_add_child(name, text);

	version = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(version, "version");
	xmpp_stanza_add_child(query, version);

	text = xmpp_stanza_new(ctx);
	xmpp_stanza_set_text(text, prog_version);
	xmpp_stanza_add_child(version, version);

	xmpp_stanza_add_child(reply, query);

	xmpp_send(conn, reply);
	xmpp_stanza_release(reply);
	return 1;
}


int message_handler(
		xmpp_conn_t * const conn, xmpp_stanza_t * const stanza,
		void * const userdata
		)
{
	xmpp_stanza_t *reply, *body, *text;
	char *intext, *replytext;
	xmpp_ctx_t *ctx = (xmpp_ctx_t*)userdata;

	if(!xmpp_stanza_get_child_by_name(stanza, "body")) return 1;
	if(!strcmp(xmpp_stanza_get_attribute(stanza, "type"), "error")) return 1;

	intext = xmpp_stanza_get_text(
			xmpp_stanza_get_child_by_name(stanza, "body")
			);

	printf_async(
			"%s: %s\n",
			xmpp_stanza_get_attribute(stanza, "from"),
			intext
			);

	reply = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(reply, "message");
	xmpp_stanza_set_type(
			reply,
			xmpp_stanza_get_type(stanza)?xmpp_stanza_get_type(stanza):"chat"
			);
	xmpp_stanza_set_attribute(
			reply, "to", xmpp_stanza_get_attribute(stanza, "from")
			);

	body = xmpp_stanza_new(ctx);
	xmpp_stanza_set_name(body, "body");

	replytext = malloc(strlen(" to you too!") + strlen(intext) + 1);
	strcpy(replytext, intext);
	strcat(replytext, " to you too!");

	text = xmpp_stanza_new(ctx);
	xmpp_stanza_set_text(text, replytext);
	xmpp_stanza_add_child(body, text);
	xmpp_stanza_add_child(reply, body);

	xmpp_send(conn, reply);
	xmpp_stanza_release(reply);
	free(replytext);
	return 1;
}

static void conn_handler(
		xmpp_conn_t * const conn, const xmpp_conn_event_t status,
		const int error, xmpp_stream_error_t * const stream_error,
		void * const userdata
		)
{
	xmpp_stanza_t* pres;

	if (status == XMPP_CONN_CONNECT) {
		printf_async("Connected.\n");
		xmpp_handler_add(conn, version_handler, "jabber:iq:version", "iq", NULL, ctx);
		xmpp_handler_add(conn, message_handler, NULL, "message", NULL, ctx);

		pres = xmpp_stanza_new(ctx);
		xmpp_stanza_set_name(pres, "presence");
		xmpp_send(conn, pres);
		xmpp_stanza_release(pres);
	} else {
		printf_async("Connection failed.\n");
		disconnect_net();
	}

}
void init_net(char* jid, char* passwd) {
	xmpp_initialize();

	/* create a context */
	/*	log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); */
	log = NULL;
	ctx = xmpp_ctx_new(NULL, log);

}

void disconnect_net() {
	if (conn != NULL) {
		xmpp_conn_release(conn);
		conn = NULL;
		printf_async("Disconnected.\n");
	}
}

void connect_net(const char* const jid, const char* const pass) {
	disconnect_net();
	printf_async("Connecting as %s.\n", jid);

	assert(!conn);
	conn = xmpp_conn_new(ctx);

	xmpp_conn_set_jid(conn, jid);
	xmpp_conn_set_pass(conn, pass);

	xmpp_connect_client(conn, NULL, 0, conn_handler, ctx);
}

void deinit_net() {
	disconnect_net();
	xmpp_ctx_free(ctx);
	xmpp_shutdown();
}

void nonblock_handle_net() {
	xmpp_run_once(ctx, 10);
}
