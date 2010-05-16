#!/usr/bin/make

CFLAGS	:= -O2 -Wall -DMODULE -D__KERNEL__ -DLINUX
CC	:= gcc
MAKE	:= make
CP      := cp
LN      := ln
RM      := rm
LD	:= ld
WHICH   := which
DOXYGEN := doxygen

static:
	$(CC)  $(CFLAGS) -o ntcalc main.c ntcalc.c

shared:
	$(CC) -fPIC -Wall -g -c ntcalc.c	
	$(LD) -shared -soname libntcalc.so.1 -o libntcalc.so.1.0 *.o

lib:	shared
	$(CP) libntcalc.so.1.0 /usr/lib/
	$(LN) -sf /usr/lib/libntcalc.so.1.0 /usr/lib/libntcalc.so
	$(CC) -o  ntcalc -Wall main.c -L/usr/lib -lntcalc

all: shared lib

install: all 
	$(CP) ntcalc   /usr/local/bin/

doc:
	$(WHICH) doxygen > /dev/null  && $(DOXYGEN) ntcalc.cfg || echo "ntcalc: doxygen not present in your system"

clean:
	$(RM) -f ntcalc
	$(RM) -f libntcalc.so.1.0 
	$(RM) -f *.o
	

