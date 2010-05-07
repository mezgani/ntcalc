#include "ntcalc.h"





/* returns number of hosts/nets from a specific prefix
 * hosts/net : 2^(32 - prefix)- 2
 */

int 
nethost(int prefix){
  return ((1 << (32 - prefix)) - 2);
}


/*
 * returns prefix of a specific netmask 
 */

int 
maskpref(uint32_t ntmask){
  
  int i = 0, prefix = 32;
  while ( i < 32) {
    if (!(ntohl(ntmask) & ((2 << i) - 1)))
      prefix--;
    i++;
  }
  return prefix;
}



/*
 * creates a netmask from a specified number of bits 
 */

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
  return htonl(0x00 - (1<<(32 - prefix)));
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
ggethosts(data_t *head, uint32_t addr, int prefix){

  data_t *first=NULL, *node=NULL;
  int i;
  
  //if (head==NULL){
    for (i=1; i<nethost(prefix)+ 1; i++){
      node = (data_t *)malloc(sizeof(data_t));
      if (i==1) {
        first = node;
      }
      node->address=((addr  & mask(prefix) )| htonl(i));
      node->counter = i;
      node->next  = head;
      printf("[%d]: [%p] %s\n", node->counter, node, int2string(node->address));
    }

  return first;
}
