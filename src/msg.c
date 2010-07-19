#include "uthash.h"
#include "msg.h"
#include "io.h"

typedef struct msg_list_node {
	char* msg;
	struct msg_list_node* next;
} *msg_list_node_t;

struct msg_queue {
	char* jid;
	msg_list_node_t first;
	msg_list_node_t last;
	UT_hash_handle hh;
};

msg_queue_t queues = NULL;
msg_queue_t active_queue = NULL;

msg_queue_t msg_queue_get(const char* jid) {
	msg_queue_t q;

	HASH_FIND_STR(queues, jid, q);

	if (!q) {
		NEW(q);
		q->jid = safe_strdup(jid);
		HASH_ADD_KEYPTR(hh, queues, q->jid, strlen(q->jid), q);
	}

	assert(q);
	return q;
}


void msg_queue_write(msg_queue_t q, char* msg) {
	msg_list_node_t new_node;

	if (q == active_queue) {
		io_message(q->jid, msg);
		FREE(msg);
		return;
	}

	if (msg_queue_empty(q)) {
		io_notification("Unread message from `%s'.", q->jid);
	}

	NEW(new_node);
	new_node->msg = msg;

	if (q->last) {
		q->last->next = new_node;
		q->last = new_node;

	} else {
		q->first = new_node;
		q->last = new_node;
	}
}

char* msg_queue_read(msg_queue_t q) {
	char* ret;
	msg_list_node_t ret_node;

	if (!q->first) {
		return NULL;
	}

	ret_node = q->first;
	q->first = q->first->next;
	
	if (q->first == NULL) {
		q->last = NULL;
	}

	ret = ret_node->msg;
	FREE(ret_node);
	return ret;
}

msg_queue_t msg_queue_iterate(msg_queue_t q) {
	if (q == NULL) {
		return queues;
	}

	return q->hh.next;
}

bool msg_queue_empty(msg_queue_t q) {
	return !q->first;
}

void msg_active_queue_set(msg_queue_t q) {
	char* msg;

	io_notification("Chat with: %s", q->jid);
	while ((msg = msg_queue_read(q))) {
		io_message(q->jid, msg);
		FREE(msg);
	}
	active_queue = q;
}

msg_queue_t msg_active_queue_get() {
	return active_queue;
}

const char* msg_queue_jid(msg_queue_t q) {
	return q->jid;
}

static void msg_list_free(msg_list_node_t n) {
	if (n) {
		msg_list_free(n->next);
		if (n->msg) {
			FREE(n->msg);
		}
		FREE(n);
	}
}

static void msg_queue_free(msg_queue_t q) {
	msg_list_free(q->first);
	FREE(q->jid);
	FREE(q);
}

void msg_init() {
	/* nothing */
}

void msg_deinit() {
	msg_queue_t curren_list;

	while (queues) {
		curren_list = queues;
		HASH_DELETE(hh, queues, curren_list);
		msg_queue_free(curren_list);
	}
}



