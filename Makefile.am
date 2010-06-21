AUTOMAKE_OPTIONS = subdir-objects

AM_CFLAGS = -g -Wall -pedantic

XMPPCONSOLE_LIBS = -lstrophe $(PARSER_LIBS) -lssl -lresolv -lreadline

## Main build targets
bin_PROGRAMS = xmppconsole

xmppconsole_CFLAGS = $(XMPPCONSOLE_FLAGS) $(AM_CFLAGS)
xmppconsole_LDADD = $(XMPPCONSOLE_LIBS)
xmppconsole_SOURCES = src/main.c src/net.c src/io.c src/cmd.c src/cmds.c
