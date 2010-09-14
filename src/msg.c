#include "msg.h"
#include "io.h"
#include "util.h"
#include "mem.h"

#include <stdlib.h>
#include <apr_hash.h>

typedef struct msg_list_node {
	char* msg;
	struct msg_list_node* next;
} msg_list_node_t;

struct msg_queue {
	char* jid;
	msg_list_node_t* first;
	msg_list_node_t* last;
};

struct msg_queue_iterator {
	apr_hash_index_t* index;
};

apr_hash_t* queues = NULL;

msg_queue_t* active_queue = NULL;

msg_queue_t* msg_queue_get(const char* jid) {
	msg_queue_t* q;

	if (!(q = apr_hash_get(queues, jid, strlen(jid)))) {
		NEW(q);
		q->jid = OOM_CHECK(strdup(jid));
		apr_hash_set(queues, q->jid, strlen(q->jid), q);
	}

	assert(q);
	return q;
}


void msg_queue_write(msg_queue_t* q, const char* msg) {
	msg_list_node_t* new_node;

	if (q == active_queue) {
		io_message(q->jid, msg);
		return;
	}

	if (msg_queue_empty(q)) {
		io_notification("Unread message from `%s'.", q->jid);
	}

	NEW(new_node);
	new_node->msg = OOM_CHECK(strdup(msg));

	if (q->last) {
		q->last->next = new_node;
		q->last = new_node;

	} else {
		q->first = new_node;
		q->last = new_node;
	}
}

char* msg_queue_read(msg_queue_t* q) {
	char* ret;
	msg_list_node_t* ret_node;

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

msg_queue_iterator_t* msg_queue_iterate(msg_queue_iterator_t* i) {

	if (i == NULL) {
		NEW(i);
		i->index = apr_hash_first(mem_main_pool, queues);
		return i;
	}

	if ((i->index = apr_hash_next(i->index)) != NULL) {
		return i;
	}
	FREE(i);
	return NULL;
}

msg_queue_t* msg_queue_iterator_get_queue(msg_queue_iterator_t* i) {
	msg_queue_t* q;

	apr_hash_this(i->index, NULL, NULL, (void*)&q);
	return q;
}

bool msg_queue_empty(msg_queue_t* q) {
	return !q->first;
}

void msg_active_queue_set(msg_queue_t* q) {
	char* msg;

	io_notification("Chat with: %s", q->jid);
	while ((msg = msg_queue_read(q))) {
		io_message(q->jid, msg);
		FREE(msg);
	}
	active_queue = q;
}

msg_queue_t* msg_active_queue_get() {
	return active_queue;
}

const char* msg_queue_jid(msg_queue_t* q) {
	return q->jid;
}

static void msg_list_free(msg_list_node_t* n) {
	if (n) {
		msg_list_free(n->next);
		if (n->msg) {
			FREE(n->msg);
		}
		FREE(n);
	}
}

static void msg_queue_free(msg_queue_t* q) {
	msg_list_free(q->first);
	FREE(q->jid);
	FREE(q);
}

void msg_init() {
	queues = apr_hash_make(mem_main_pool);
}

void msg_deinit() {
	apr_hash_index_t* hi;
	apr_pool_t* p = mem_main_pool;
	msg_queue_t* q;

	for (hi = apr_hash_first(p, queues); hi; hi = apr_hash_next(hi)) {
		apr_hash_this(hi, NULL, NULL, (void*)&q);
		msg_queue_free(q);
	}

	apr_hash_clear(queues);
}



