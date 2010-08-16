#ifndef __XC_ROSTER_H__
#define __XC_ROSTER_H__

typedef struct roster_item *roster_item_t;

typedef enum subscription_state {
	SUBSCRIPTION_NONE,
	SUBSCRIPTION_TO,
	SUBSCRIPTION_FROM,
	SUBSCRIPTION_BOTH
} subscription_state_t;

roster_item_t roster_item_received(
		const char* jid, const char* name, subscription_state_t sub
		);
roster_item_t roster_iterate(roster_item_t i);
char* roster_item_get_name(roster_item_t i);
char* roster_item_get_jid(roster_item_t i);
subscription_state_t roster_item_get_subscription(roster_item_t i);

subscription_state_t roster_subscription_to_enum(const char* str);

void roster_init();
void roster_deinit();

#endif
