# Enter you UVAID here
UVAID=10208917
PROGNAME=check

# Commands for creating the submit archive
MKDIR=mkdir
PACK=tar -jcvf
CP=cp
RM=rm -rf

CC=gcc
# You can use clang as an alternative compiler
#CC=clang

CFLAGS=  -std=c99 -Wall -Wextra -pedantic
# debug flags.
#CFLAGS= -g3 -DDEBUG -std=c99 -Wall -Wextra -pedantic

all: check

chesk: check.o

chesk.o: debug.h

run: check
	./chess < board1.txt

clean:
	rm -f *.o check

tarball: all
	$(MAKE) clean
	$(MKDIR) $(PROGNAME)-$(UVAID)
	$(CP) * $(PROGNAME)-$(UVAID) || true
	$(PACK) $(PROGNAME)-$(UVAID).tbz $(PROGNAME)-$(UVAID)/*
	$(RM) $(PROGNAME)-$(UVAID)
