#ifndef __XC_CMD_H__
#define __XC_CMD_H__

#include <stdbool.h>

/**
 * Parsed command state
 */
typedef struct cmd_tokenized_node {
	char* name;
	struct cmd_tokenized_node *next;
} cmd_tokenized_node_t;

typedef struct cmd_descriptor {
	char* name;
	void (*handle)(const cmd_tokenized_node_t*);
	struct cmd_descriptor** sub; /* subcommands */
	void* autocompleter;
} cmd_descriptor_t;


void interprete(const char * const str);

char** cmd_root_autocompleter(
		const char *text,
		int start, int end
		);

void cmd_execute(const char* line);

#endif
