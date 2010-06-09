# Introduction

This is (or going to be) raw and simple, but handy XMPP client for Unix
command-line.

Readline library is used in a neat way to server asynchronous display
(see ./src/io.c). It looks very nice, really - clean and simple.

Libstrophe library is used to handle XMPP communication.

# Status

It's fresh and new. Please expect bugs. The code could receive a little cleanup
but is quite consistent and mostly trivial.

**I encourage you to collaborate and send me some pull requests!**

What is working:

* /connect <jid> <pass> - attempts to connect to XMPP server
* /debug - switches debug logging
* @<jid> - sets current chat recipient
* Hi! - sens "Hi!" to current message recipient
* /disconnect
* /quit


TODO:

* display notifications about new communication only
* display received messages after selecting sender as a current recipient
* jid aliasing (stored server side?)
* config file
* roster manipulation
* more functionality (MUC, PEP, other XEPs)
