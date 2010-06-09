void net_init();
void net_nonblock_handle();
void net_connect(const char* const jid, const char* const pass);
void net_disconnect();
void net_deinit();
void net_set_current_recipent(const char* const jid);
void net_send(const char* const str);
