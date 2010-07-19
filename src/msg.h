#ifndef __XC_MSG_H__
#define __XC_MSG_H__

#include "util.h"

typedef struct msg_queue *msg_queue_t;

void msg_init();
void msg_deinit();

msg_queue_t msg_queue_get(const char* jid);
void msg_queue_write(msg_queue_t q, char* msg);
char* msg_queue_read(msg_queue_t q);
const char* msg_queue_jid(msg_queue_t q);
bool msg_queue_empty(msg_queue_t q);
void msg_active_queue_set(msg_queue_t q);
msg_queue_t msg_active_queue_get();
msg_queue_t msg_queue_iterate(msg_queue_t q);

#endif
