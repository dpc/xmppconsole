AUTOMAKE_OPTIONS = subdir-objects

GIT_VERSION = $(shell sh -c 'git describe --dirty | sed s/^v//')
DEFS += -DGIT_VERSION=\"$(GIT_VERSION)\"

## Main build targets
bin_PROGRAMS = xmppconsole

xmppconsole_CFLAGS = @LIBSTROPHE_CFLAGS@ @LIBAPR_CFLAGS@ -Wno-uninitialized -Wall -fPIC -std=gnu99 -pedantic
xmppconsole_LDADD = @LIBSTROPHE_LIBS@ @LIBAPR_LIBS@ -lreadline -llua -ldl
xmppconsole_SOURCES = src/main.c src/net.c src/io.c src/cmd.c \
	src/cmds.c src/util.c src/msg.c src/roster.c src/mem.c \
	src/li.c
