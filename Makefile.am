AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = @AM_CFLAGS@ -Wno-uninitialized -Wall -fPIC -std=gnu99

GIT_VERSION := $(shell sh -c 'git describe --dirty | sed s/^v//')
DEFS += -DGIT_VERSION=\"$(GIT_VERSION)\"

XMPPCONSOLE_CFLAGS = $(AM_CFLAGS) @LIBSTROPHE_CFLAGS@
XMPPCONSOLE_LIBS = @LIBSTROPHE_LIBS@ -lreadline

## Main build targets
bin_PROGRAMS = xmppconsole

xmppconsole_CFLAGS = $(XMPPCONSOLE_FLAGS) $(AM_CFLAGS)
xmppconsole_LDADD = $(XMPPCONSOLE_LIBS)
xmppconsole_SOURCES = src/main.c src/net.c src/io.c src/cmd.c \
	src/cmds.c src/util.c src/msg.c



