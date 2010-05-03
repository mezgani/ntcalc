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
//#include <regex.h>


//#define CIDR "([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\/([0-9]{1,2})"
//#define CIDR ([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])\.([01]?\d\d?|2[0-4]\d|25[0-5])/([0-9]{1,2})


static 
void die (char *msg)
{
  if (errno != 0) {
    fprintf(stderr, strerror(errno));
  }
 
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

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
  return (((addr & mask(prefix)) | ~(mask(prefix)) -(htonl(1))));
  
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



static 
void usage(FILE *out, char *program)
{
  fprintf(out, "\n\t\t%s  [address/prefix] [-v][-h]\n",program);
  fprintf(out, "\t\t%s -n 192.168.1.2/24  \n\n", program);
}


int main(int argc, char **argv)
{

  char *address, *pre;
  uint32_t a,b,c,d,e,f;
  unsigned long value;
  //regex_t re;
  int prefix;

  int var1 = 0, var2 = 0;
  int var3 = 0, var4 = 0;
  int var5 = 0;

  if (argc != 2)
    {
      usage(stderr, argv[0]);
      exit(0);

    }
  
  sscanf(argv[1], "%i.%i.%i.%i/%i", &var1, &var2, &var3, &var4, &var5);
  if ((var1>255) || (var2>255) || (var3>255) || (var4>255) || (var5>32)){
    die("Not an IPv4 dotted quad or CIDR notation\n");
  }
    
  
  if (strchr(argv[1], '/')==NULL) {
    die("Not an IPv4 dotted quad or CIDR notation\n");
  }


  address=strtok(argv[1], "/");
  pre =strtok(NULL, "/");
  prefix=atoi(pre);

  
  value = string2int(address);


  /*   
      if(regcomp(&re, CIDR, REG_EXTENDED) != 0) {
      die("Error while compiling regular expression\n");
      }
      if(regexec(&re, address, 0, NULL, 0) != 0) {
      die("Not an IPv4 dotted quad or CIDR notation\n");
      }
  */
  

  



  a=netmask(prefix);
  b=broadcast(value, prefix);
  c=network(value, prefix);
  d=wildcard(prefix);
  e=hostmin(value, prefix);
  f=hostmax(value, prefix);


  // I like output color :)

  printf("\033[1;34m*\033[0m");
  printf(" Address      : %s\n", address);
  printf("\033[1;34m*\033[0m");
  printf(" Netmask      : %s\n", int2string(a));
  printf("\033[1;34m*\033[0m");
  printf(" Broadcast    : %s\n", int2string(b));
  printf("\033[1;34m*\033[0m");
  printf(" Wildcard     : %s\n", int2string(d));
  printf("\033[1;34m*\033[0m");
  printf(" Network      : %s/%s\n", int2string(c),pre);
  printf("\033[1;34m*\033[0m");
  printf(" Hostmin      : %s\n", int2string(e));
  printf("\033[1;34m*\033[0m");
  printf(" Hostmax      : %s\n", int2string(f));
  printf("\033[1;34m*\033[0m");
  printf(" Hosts/nets   : %d\n", nethost(prefix));



  return 0;
}







