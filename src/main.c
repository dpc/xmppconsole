#include "io.h"
#include "li.h"
#include "net.h"
#include "msg.h"
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int prog_running = 1;
const char* const prog_version = GIT_VERSION;
const char* const prog_author = "dpc <dpc@ucore.info>";
const char* const prog_name = "xmppconsole";

int main() {

	io_init();
	li_init();
	msg_init();
	net_init();

	io_printfln("Welcome in %s, %s by %s.", prog_name, prog_version, prog_author);
	io_printfln("Type /help for assistance.");

	while (prog_running) {
		io_nonblock_handle();
		net_nonblock_handle();
	}

	io_printfln("Exiting...");
	net_deinit();
	msg_deinit();
	li_deinit();
	io_deinit();
	return 0;
}
