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
		cmd_tokenized_node_t* state,
		const char* line,
		const char* stop
		) {
	const char* end;
	cmd_tokenized_node_t new_token = NULL;
	(*state) = NULL;

	while (line < stop && *line && isspace(*line)) {
		++line;
	}

	if (!line[0] || line >= stop) {
		return;
	}

	end = line;

	NEW(new_token);
	*state = new_token;

	while (end < stop && *end && !isspace(*end)) {
		++end;
	}

	new_token->name = safe_strndup(line, end - line);

	cmd_tokenize_int(&(new_token->next), end, stop);
}

/**
 * Tokenize a string into list of tokens.
 */
static cmd_tokenized_node_t cmd_tokenize(const char* start, const char* stop) {
	cmd_tokenized_node_t token = NULL;

	cmd_tokenize_int(&token, start, stop);

	return token;
}


/**
 * Free list created by cmd_tokenize
 */
static void cmd_tokenized_free(cmd_tokenized_node_t list) {
	if (!list) {
		return;
	}

	cmd_tokenized_free(list->next);

	free(list);
}

typedef struct cmd_traverse_state {
	cmd_descriptor_t* root;
	cmd_descriptor_t cmd;
	cmd_tokenized_node_t token;
} *cmd_traverse_state_t;

/**
 * Travers cmd descr tree using tokens.
 */
static void cmd_traverse(
		cmd_traverse_state_t state
		) {

	cmd_descriptor_t* i = state->root;

	if (state->token == NULL) {
		return;
	} else if (state->root == NULL) {
		return;
	}


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

static cmd_descriptor_t* cmd_subcmd_matches_generator_cur_root = NULL;

char* cmd_subcmd_matches_generator(
		const char* text,
		int state
		) {
	cmd_descriptor_t* ret = NULL;

	while (*cmd_subcmd_matches_generator_cur_root) {
		ret = cmd_subcmd_matches_generator_cur_root;
		cmd_subcmd_matches_generator_cur_root++;
		if (strncmp((*ret)->name, text, strlen(text)) == 0) {
			return safe_strdup((*ret)->name);
		}
	}

	return NULL;
}

static cmd_descriptor_t cmd_customcompleter_matches_generator_cur_cmd = NULL;
static cmd_tokenized_node_t cmd_customcompleter_matches_generator_tokens = NULL;
static int cmd_customcompleter_matches_generator_i = 0;

char* cmd_customcompleter_matches_generator(
		const char* text,
		int state
		) {
	char* str;

	while ((str = cmd_customcompleter_matches_generator_cur_cmd->completer(
				cmd_customcompleter_matches_generator_i++,
				cmd_customcompleter_matches_generator_tokens
				))) {
		if (strncmp(str, text, strlen(text)) == 0) {
			return safe_strdup(str);
		}
	}

	return NULL;
}


char** cmd_root_autocompleter(
		const char *text,
		int start, int end
		) {
	cmd_tokenized_node_t tokens = NULL;
	struct cmd_traverse_state state;
	char** matches = NULL;

	rl_attempted_completion_over = 1;

	tokens = state.token = cmd_tokenize(rl_line_buffer, rl_line_buffer + start);
	state.root = cmds_root;
	state.cmd = NULL;
	cmd_traverse(&state);

	if (state.cmd && state.cmd->completer) {
		cmd_customcompleter_matches_generator_cur_cmd = state.cmd;
		cmd_customcompleter_matches_generator_i = 0;
		cmd_customcompleter_matches_generator_tokens = tokens;
		matches = rl_completion_matches(text, cmd_customcompleter_matches_generator);
	} else if (state.root) {
		cmd_subcmd_matches_generator_cur_root = state.root;
		matches = rl_completion_matches(text, cmd_subcmd_matches_generator);
	}

	cmd_tokenized_free(tokens);

	return matches;
}

static void cmd_execute_slash(const char* s) {
	cmd_tokenized_node_t tokens = NULL;
	struct cmd_traverse_state state;

	tokens = state.token = cmd_tokenize(s, s + strlen(s));
	state.root = cmds_root;
	state.cmd = NULL;
	cmd_traverse(&state);

	if (!state.cmd) {
		io_printfln("Unknown command: `%s'", state.token->name);
		goto ret;
	}

	if (state.cmd->handle) {
		state.cmd->handle(state.token);
	} else {
		io_printfln("Unknown command: %s", state.token->name);
		goto ret;
	}

ret:
	cmd_tokenized_free(tokens);
}

void cmd_execute(const char* str) {
	if (strcmp(str, "") == 0) {
		return;
	}

	if (str[0] == '/') {
		if (io_debug_get()) {
			io_printfln("%s%s", io_prompt_get(), str);
		}
		cmd_execute_slash(str);
	} else {
		cmds_default(str);
	}
}

