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

/* data structure that will contains hosts/nets in linked list 
 *
 */

struct data {
  uint32_t address ;
  int      counter ;
  struct data *next;
};


/* data type named data_t
 *
 */
typedef struct data data_t;


int nethost(int prefix);
int maskpref(uint32_t mask);
uint32_t  mask(int prefix);
uint32_t  netmask(int prefix);
uint32_t wildcard(int prefix);
uint32_t broadcast(uint32_t addr, int prefix);
uint32_t network(uint32_t addr, int prefix);
uint32_t hostmin(uint32_t addr, int prefix);
uint32_t hostmax(uint32_t addr, int prefix);
char *int2string(uint32_t address);
uint32_t string2int(char *address);
data_t *gethosts(uint32_t addr, int prefix);
void display(data_t *current);
data_t *ggethosts(data_t *head, uint32_t addr, int prefix);


