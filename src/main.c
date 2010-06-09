#include "io.h"
#include "net.h"
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int prog_running = 1;
const char* const prog_version = "0.1";
const char* const prog_name = "xmppconsole";

int main() {
	int c = 1;

	printf("Start.\n");
	init_io();
	init_net();

	while (prog_running) {

		if (((++c) % 5000) == 0) {
			printf_async("Test message: %d\n", c);
		}
		nonblock_handle_io();
		nonblock_handle_net();
	}

	printf_async("Exiting...\n");
	deinit_net();
	deinit_io();
	return 0;
}
