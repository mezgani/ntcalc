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




/* returns number of hosts/nets from a specific prefix
 * hosts/net : 2^(32 - prefix)- 2
 */

int 
nethost(int prefix){
  return ((1 << (32 - prefix)) - 2);
}

/* creates a netmask from a specified number of bits */

uint32_t 
mask(int prefix) {
  return htonl(~((1 << (32 - prefix))- 1));
}


/* returns the default (canonical) netmask associated with specified IP address.
 * as uint32_teger in network byte order.
 */

uint32_t 
netmask(int prefix)
{
  return htonl(0x00 - (1<<(32 - prefix - 1)));
}

/* returns the default wildcard associated with specifed prefix
 *
 */

uint32_t 
wildcard(int prefix)
{
  return htonl(0xFFFFFFFF +  (1<<(32 - prefix)));
}


/* calculate broadcast address given an IP address and a prefix length. 
 *  broadcast: address or wildcard 
 */

uint32_t 
broadcast(uint32_t addr, int prefix)
{
  return ((addr & mask(prefix)) | ~mask(prefix)) ;
}


/* calculates the network address for a specified address and prefix.
 * network: address and netmask
 */

uint32_t 
network(uint32_t addr, int prefix)
{
  return (addr & mask(prefix));
}

/* returns host min for a specific address and prefix
 * hostmin: network + 1
 */

uint32_t 
hostmin(uint32_t addr, int prefix)
{
  return ((addr  & mask(prefix)) | htonl(1)); 
 
}

/* returns host max for a specific address and prefix
 * hostmax: broadcast - 1
 */

uint32_t 
hostmax(uint32_t addr, int prefix)
{
  return (((addr & mask(prefix)) | ~mask(prefix)) -htonl(1));
  
}


/* retuns (char *) for an uint32_t address given.
 *
 */

char *
int2string(uint32_t address)
{

  struct in_addr inr;
  inr.s_addr=address;
  return inet_ntoa(inr);
}

/* returns an uint32_t from a string address given.
 * 
 */

uint32_t 
string2int(char *address)
{
  return inet_addr(address);
}


/* returns a current pointer to data_t type, after that gethosts append address to the list
 * 
 */

data_t *gethosts(uint32_t addr, int prefix)
{
  data_t *current, *head;
  int i;
  
  current = head = NULL;

  for (i=1; i<nethost(prefix)+1;i++){
    current = (data_t *)malloc(sizeof(data_t));
    current->address=((addr  & mask(prefix) )| htonl(i));
    current->counter = i;
    current->next  = head;
    head = current;
  }
  return current;

}

/* display function that take two pointers current and header of list
 *
 */

void 
display(data_t *current) {
  if (current == NULL)
    {
      printf("The list is empty!\n");
    }
  do
    {
      printf("[%3d]: %15s\n", current->counter, 
	     int2string(current->address));   

      current = current->next;
    }
  while (current);
}


/* returns a pointer to head of our struct, after that gethosts append addresses to the list
 * 
 */

data_t *
gggethosts(data_t *head, uint32_t addr, int prefix){

  data_t *first=NULL, *node=NULL;
  int i;
  
  if (head==NULL){
    for (i=1; i<nethost(prefix)+ 1; i++){
      node = (data_t *)malloc(sizeof(data_t));
      if (i==1) {
        first = node;
      }
      node->address=((addr  & mask(prefix) )| htonl(i));
      node->counter = i;
      node->next  = head;
    }
    for (i=1; i<nethost(prefix)+ 1; i++){
      node = (data_t *)malloc(sizeof(data_t));
      node->address=((addr  & mask(prefix) )| htonl(i));
      node->counter = i;
      node->next = NULL;
      
    }
  }
  return first;
}
