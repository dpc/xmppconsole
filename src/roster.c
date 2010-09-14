#include "roster.h"

#include "uthash.h"
#include "util.h"
#include <stdlib.h>

struct roster_item {
	char* jid;
	char* name;
	subscription_state_t subscription;
	UT_hash_handle hh;
};

static roster_item_t* roster = NULL;


roster_item_t* roster_item_received(
		const char* jid,
		const char* name,
		subscription_state_t sub
		) {
	roster_item_t* i = NULL;
	roster_item_t* n = NULL;

	HASH_FIND_STR(roster, jid, i);

	if (!i) {
		NEW(n);
		i = n;
	}
	if (i->jid) {
		FREE(i->jid);
	}
	if (i->name) {
		FREE(i->name);
	}
	i->jid = OOM_CHECK(strdup(jid));
	i->name = OOM_CHECK(strdup(name));
	i->subscription = sub;

	if (n) {
		HASH_ADD_KEYPTR(hh, roster, i->jid, strlen(i->jid), i);
	}

	return i;
}

subscription_state_t roster_subscription_to_enum(const char* str) {
	if (strcmp(str, "none") == 0) {
		return SUBSCRIPTION_NONE;
	} else if (strcmp(str, "both") == 0) {
		return SUBSCRIPTION_BOTH;
	} else if (strcmp(str, "to") == 0) {
		return SUBSCRIPTION_TO;
	} else if (strcmp(str, "from") == 0) {
		return SUBSCRIPTION_FROM;
	}

	/* TODO: reconsider this */
	return SUBSCRIPTION_NONE;
}

char* roster_item_get_name(roster_item_t* i) {
	return OOM_CHECK(strdup(i->name));
}

char* roster_item_get_jid(roster_item_t* i) {
	return OOM_CHECK(strdup(i->jid));
}

subscription_state_t roster_item_get_subscription(roster_item_t* i) {
	return i->subscription;
}

roster_item_t* roster_iterate(roster_item_t* i) {
	if (i == NULL) {
		return roster;
	}

	return i->hh.next;
}

void roster_item_free(roster_item_t* i) {
	free(i->jid);
	free(i->name);
	free(i);
}

void roster_init() {
	/* nothing to do */
}

void roster_deinit() {
	roster_item_t* current_item;

	while (roster) {
		current_item = roster;
		HASH_DELETE(hh, roster, current_item);
		roster_item_free(current_item);
	}
}


