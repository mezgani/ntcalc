/*
 * __software__= "ntcalc"
 * __author__ = "MEZGANI Ali mezgani@nativelabs.org"
 * __version__ = "0.1"
 * __date__ = "2010/04/01 23:46:07"
 * __credits__ = "Thanks mam for support"
 *
 * ntcalc is a program that provide ip address and network calculations, it support actually only IPv4.
 * ntcalc is Free Software you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation; either version 3 of the License,
 * or (at your option) any later version.See the GNU General Public License for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

/** 
 * LIFO linked list data structure that will contains hosts/nets
 */

struct data {
  uint32_t address ;
  int      counter ;
  struct data *next;
};


/**
 *  data type named data_t
 */
typedef struct data data_t;





/** returns number of hosts/nets from a specific prefix
 *  hosts/net : 2^(32 - prefix)- 2
 */

int nethost(int prefix);


/**
 * returns prefix of a specific netmask 
 */

int maskpref(uint32_t ntmask);

/**
 * creates a netmask from a specified number of bits 
 */

uint32_t mask(int prefix);


/** returns the default (canonical) netmask associated with specified IP address.
 * as uint32_teger in network byte order.
 */

uint32_t netmask(int prefix);


/**
 *  returns the default wildcard associated with specifed prefix
 */

uint32_t wildcard(int prefix);


/** calculate broadcast address given an IP address and a prefix length. 
 *  broadcast: address or wildcard 
 */

uint32_t broadcast(uint32_t addr, int prefix);


/** calculates the network address for a specified address and prefix.
 *  network: address and netmask
 */

uint32_t network(uint32_t addr, int prefix);


/** returns host min for a specific address and prefix
 *  hostmin: network + 1
 */

uint32_t hostmin(uint32_t addr, int prefix);


/** returns host max for a specific address and prefix
 *  hostmax: broadcast - 1
 */

uint32_t hostmax(uint32_t addr, int prefix);



/** 
 *  retuns (char *) for an uint32_t address given.
 */

char *int2string(uint32_t address);

/** 
 *  returns an uint32_t from a string address given. 
 */

uint32_t string2int(char *address);


/** 
 * returns a pointer to data_t type, after appending address to the list 
 */

data_t *gethosts(uint32_t addr, int prefix);



/** 
 * display function that take two pointers current and header of list
 */

void display(data_t *current);




