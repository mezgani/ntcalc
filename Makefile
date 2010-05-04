#!/usr/bin/make

CFLAGS	:= -O2 -Wall -DMODULE -D__KERNEL__ -DLINUX
CC	:= gcc
MAKE	:= make
CP      := cp
LN      := ln
RM      := rm
LD	:= ld

static:
	$(CC)  $(CFLAGS) -o ntcalc main.c

shared:
	$(CC) -fPIC -Wall -g -c inetcalc.c	
	$(LD) -shared -soname libntcalc.so.1 -o libntcalc.so.1.0 *.o

lib:	shared
	$(CP) libntcalc.so.1.0 /usr/lib/
	$(LN) -sf /usr/lib/libntcalc.so.1.0 /usr/lib/libntcalc.so
	$(CC) -o  ntcalc -Wall main.c -L/usr/lib -lntcalc

all: shared lib

install: all 
	$(CP) ntcalc   /usr/local/bin/

clean:
	$(RM) ntcalc
	$(RM) libntcalc.so.1.0 
	$(RM) *.o
