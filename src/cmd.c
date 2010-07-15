#include "cmd.h"
#include "cmds.h"
#include "io.h"
#include "util.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>

/**
 * Tokenize recursively a line.
 */
static void cmd_tokenize_int(
		cmd_tokenized_node_t** state,
		const char* line,
		const char* stop
		) {
	const char* end;
	cmd_tokenized_node_t* new_token = NULL;
	(*state) = NULL;

	while (line < stop && *line && isspace(*line)) {
		++line;
	}

	if (!line[0] || line >= stop) {
		return;
	}

	end = line;

	*state = new_token = safe_malloc(sizeof(cmd_tokenized_node_t));
	memset(new_token, 0, sizeof(*new_token));

	while (end < stop && *end && !isspace(*end)) {
		++end;
	}

	new_token->name = safe_strndup(line, end - line);

	cmd_tokenize_int(&(new_token->next), end, stop);
}

/**
 * Tokenize a string into list of tokens.
 */
cmd_tokenized_node_t* cmd_tokenize(const char* start, const char* stop) {
	cmd_tokenized_node_t* token = NULL;

	cmd_tokenize_int(&token, start, stop);

	return token;
}


/**
 * Free list created by cmd_tokenize
 */
static void cmd_tokenized_free(cmd_tokenized_node_t* list) {
	if (!list) {
		return;
	}

	cmd_tokenized_free(list->next);

	free(list);
}

typedef struct cmd_traverse_state {
	cmd_descriptor_t** root;
	cmd_descriptor_t* cmd;
	cmd_tokenized_node_t* token;
} cmd_traverse_state_t;

/**
 * Travers cmd descr tree using tokens.
 */
static void cmd_traverse(
		cmd_traverse_state_t* state
		) {

	cmd_descriptor_t** i = state->root;

	if (state->token == NULL) {
		return;
	} else if (state->root == NULL) {
		return;
	}


	io_debugln("looking for a match for `%s' token", state->token->name);
	while (*i != NULL) {
		if (strcmp((*i)->name, state->token->name) == 0) {
			state->root = (*i)->sub;
			state->cmd = *i;
			state->token = state->token->next;
			cmd_traverse(state);
			return;
		}
		i++;
	}

	state->root = NULL;
}

static cmd_descriptor_t** cmd_matches_generator_cur_root = NULL;

char* cmd_matches_generator(
		const char* text,
		int state
		) {
	cmd_descriptor_t** ret = NULL;

	while (*cmd_matches_generator_cur_root) {
		ret = cmd_matches_generator_cur_root;
		cmd_matches_generator_cur_root++;
		if (strncmp((*ret)->name, text, strlen(text)) == 0) {
			return safe_strdup((*ret)->name);
		}
	}

	return NULL;
}

char** cmd_root_autocompleter(
		const char *text,
		int start, int end
		) {
	cmd_tokenized_node_t* tokens = NULL;
	cmd_traverse_state_t state;
	char** matches = NULL;

	rl_attempted_completion_over = 1;

	/* no completion for lines not starting with / */
	if (strncmp("/", rl_line_buffer, 1) != 0) {
		return NULL;
	}

	tokens = state.token = cmd_tokenize(rl_line_buffer, rl_line_buffer + start);
	state.root = cmds_root;
	state.cmd = NULL;
	cmd_traverse(&state);

	if (state.cmd) {
		io_debugln("cmd: %s", state.cmd->name);
	}

	if (!state.root) {
		io_debugln("NULL root - nothing to complete");
	} else {
		io_debugln("root: %d-%d: %s", start, end, text);

		cmd_matches_generator_cur_root = state.root;
		matches = rl_completion_matches(text, cmd_matches_generator);
	}

	cmd_tokenized_free(tokens);

	return matches;
}

static void cmd_execute_slash(const char* s) {
	cmd_tokenized_node_t* tokens = NULL;
	cmd_traverse_state_t state;

	tokens = state.token = cmd_tokenize(rl_line_buffer, rl_line_buffer + strlen(rl_line_buffer));
	state.root = cmds_root;
	state.cmd = NULL;
	cmd_traverse(&state);

	if (!state.cmd) {
		io_printfln("Unknown command: %s", state.token->name);
		goto ret;
	}

	if (state.cmd->handle) {
		state.cmd->handle(state.token);
	} else {
		io_printfln("Unknown command: %s", state.token->name);
		goto ret;
	}
	/* delme
	while (state->name) {
		if (strncmp(state->name, text, strlen(text)) == 0) {
			return safe_strdup(ret->name);
		}
		state++;
	}

	for (i = 0; i < sizeof(cmds_root) / sizeof(struct cmd_t); ++i) {
		if (strcmp(cmds_root[i].name, cmd) == 0) {
			cmds_root[i].handle(str);
			goto ret;
		}
	}
*/
ret:
	cmd_tokenized_free(tokens);
}

void cmd_execute(const char* str) {
	if (strcmp(str, "") == 0) {
		return;
	}

	if (str[0] == '/') {
		cmd_execute_slash(str);
	} else {
		cmds_default(str);
	}
}

