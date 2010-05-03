/*
__software__= "ntcalc"  
__author__ = "MEZGANI Ali mezgani@nativelabs.org"                                                                               
__version__ = "0.1"                                                                                                                                               
__date__ = "2010/04/01 23:46:07"
__credits__ = "Thanks mam for support" 
                                                                                                                                            
ntcalc is a program that provide ip address and network calculations, it support actually only IPv4.
ntcalc is Free Software you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation; either version 3 of the License, 
or (at your option) any later version.See the GNU General Public License for more details.                                                                    
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>



/* return number of hosts/nets from a specific prefix
 * hosts/net : 2^(32 - prefix)- 2
 */

int nethost(int prefix){
  return ((1 << (32 - prefix)) - 2);
}

/* creates a netmask from a specified number of bits */

uint32_t mask(int prefix) {
    return htonl(~((1 << (32 - prefix)) - 1));
}


/* returns the default (canonical) netmask associated with specified IP address.
 * as uint32_teger in network byte order.
 */

uint32_t netmask(int prefix)
{
  return htonl(0x00 - (1<<(32 - prefix)));
}


uint32_t wildcard(int prefix)
{
  return htonl(0xff +  (1<<(32 - prefix)));
}


/* calculate broadcast address given an IP address and a prefix length. 
 *  broadcast: address or wildcard 
 */

uint32_t broadcast(uint32_t addr, int prefix)
{
  return ((addr & mask(prefix)) | ~mask(prefix));
}


/* calculates the network address for a specified address and prefix.
 * network: address and netmask
 */

uint32_t network(uint32_t addr, int prefix)
{
  return (addr & mask(prefix));
}

/* return host min for a specific address and prefix
 * hostmin: network + 1
 */

uint32_t hostmin(uint32_t addr, int prefix)
{
  return ((addr & mask(prefix)) & 0xFFFFFFF1); 

}

/* return host max for a specific address and prefix
 * hostmax: broadcast - 1
 */

uint32_t hostmax(uint32_t addr, int prefix)
{
  return (((addr & mask(prefix)) | ~mask(prefix)) & 0xFFFFFFF1);
  
}


char *int2string(uint32_t address)
{

  struct in_addr inr;
  inr.s_addr=address;
  return inet_ntoa(inr);
}

uint32_t string2int(char *address)
{
  return inet_addr(address);
}



int main(int argc, char **argv)
{


  char *address="192.168.201.1";
  int prefix=22;
  uint32_t x,y,z,a,b,c;
 
  unsigned long value = string2int(address);

  x=netmask(prefix);
  y=broadcast(value, prefix);
  z=network(value, prefix);
  a=hostmin(value, prefix);
  b=hostmax(value, prefix);
  c=wildcard( prefix);

  printf("netmask   : %s\n", int2string(x));
  printf("broadcast : %s\n", int2string(y));
  printf("wildcard  : %s\n", int2string(c));
  printf("network   : %s\n", int2string(z));
  printf("hostmin   : %s\n", int2string(a));
  printf("hostmax   : %s\n", int2string(b));
  printf("hosts/nets: %d\n", nethost(prefix));


  return 0;
}







