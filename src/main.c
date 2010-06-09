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

	printf("Start.\n");
	io_init();
	net_init();

	while (prog_running) {
		io_nonblock_handle();
		net_nonblock_handle();
	}

	printf_async("Exiting...\n");
	net_deinit();
	io_deinit();
	return 0;
}
