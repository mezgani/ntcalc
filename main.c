#include "ntcalc.h"

static 
void usage(FILE *out, char *program)
{

  fprintf(out, "\033[0;34mUsage:\t  \%s    [address]/[prefix]\n",program);
  fprintf(out, "Example:  %s     192.168.1.2/24\n\n\033[0m", program);
  fprintf(out, "\033[0;31mntcalc (network calculator) takes an IPv4 address and a prefix and calculates \nthe resulting broadcast, network, Cisco wildcard mask, and host/net range.\n\033[0m");

  
}


static 
void die (char *msg)
{
  if (errno != 0) {
    fprintf(stderr, strerror(errno));
  }
 
  fprintf(stderr, "%s\n", msg);
  exit(1);
}



int main(int argc, char **argv)
{

  char *address, *pre;
  //data_t *ptr=NULL;
  uint32_t a,b,c,d,e,f;
  unsigned long value;
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

  /*
    ptr=gethosts(value, prefix);

  if (ptr==NULL){
    die("Emty data\n");
  }    
  display(ptr);
  */

  return 0;
}







