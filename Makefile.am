AUTOMAKE_OPTIONS = subdir-objects

CFLAGS = -g -Wall -pedantic

builtin_LIBS = -lexpat

PARSER_CFLAGS=@PARSER_CFLAGS@
PARSER_LIBS=@PARSER_LIBS@

XMPPCONSOLE_LIBS = -lstrophe $(PARSER_LIBS) -lssl -lresolv -lreadline

## Main build targets
bin_PROGRAMS = xmppconsole

xmppconsole_CFLAGS = $(XMPPCONSOLE_FLAGS)
xmppconsole_LDADD = $(XMPPCONSOLE_LIBS)
xmppconsole_SOURCES = src/main.c src/net.c src/io.c src/cmd.c src/cmds.c
