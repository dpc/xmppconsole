#ifndef __XC_NET_H__
#define __XC_NET_H__

typedef enum {
	NET_ST_DISCONNECTED,
	NET_ST_OFFLINE,
	NET_ST_ONLINE
} net_status_t;

void net_init();
void net_nonblock_handle();
void net_connect(const char* const jid, const char* const pass);
void net_disconnect();
void net_deinit();
void net_send(const char* const str);

#endif
