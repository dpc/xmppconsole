AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -O0 -g -Wall -pedantic -pedantic-errors -fPIC -std=gnu99

XMPPCONSOLE_CFLAGS = $(AM_CFLAGS)
XMPPCONSOLE_LIBS = -lstrophe $(PARSER_LIBS) -lssl -lresolv -lreadline

## Main build targets
bin_PROGRAMS = xmppconsole

xmppconsole_CFLAGS = $(XMPPCONSOLE_FLAGS) $(AM_CFLAGS)
xmppconsole_LDADD = $(XMPPCONSOLE_LIBS)
xmppconsole_SOURCES = src/main.c src/net.c src/io.c src/cmd.c \
	src/cmds.c src/util.c src/msg.c

xmppconsoledir = "./"
xmppconsole_DATA = src/ver.h

src/ver.h: $(xmppconsole_SOURCES)
	echo -n "#define PROG_VER \"" > src/ver.h
	git describe | tr -d '\n' >> src/ver.h
	echo "\"" >> src/ver.h
