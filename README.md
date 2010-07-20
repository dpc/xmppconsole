# Introduction

`xmppconsole` goal is to be raw and simple, but very handy XMPP client for Unix
command-line.

Readline library is used in a neat way to server asynchronous display
(see ./src/io.c). It looks very nice, really - clean and simple.

Libstrophe library is used to handle XMPP communication.

# Status

Project is still fresh and immature. Please expect bugs and continuous
code changes. However it allows communication.

# Building

## Dependences

xmppconsole requires:

* readline (bundled with a system)
* libstrophe

`readline` is so common that all *nix distributions provide it in
standard installation. Libstrophe is a C library that can be downloaded from:
http://code.stanziq.com/strophe/ . If you have any problems with it you
may try to use my libstrophe fork:
http://github.com/dpc/libstrophe .

## Compilation

After cloning from git and installing all dependences:

	./bootstrap.sh
	./configure.sh [options]
	make
	make install

# Running

In terminal run:

	$ xmppconsole

and just follow the instructions.
