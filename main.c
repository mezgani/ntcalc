#include <regex.h>
#include "ntcalc.h"






#define IPEXPR  "([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})"
#define CIDR  "([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})/([0-9]{1,2})"
#define PREFIX  "([0-9]{1,2})"





static 
void usage(FILE *out, char *program)
{
  fprintf(out, "\033[0;31m\nntcalc (network calculator) takes an IPv4 address and a prefix and calculates\n\
the resulting broadcast, network, Cisco wildcard mask, and host/net range.\n\n\033[0m");
  fprintf(out, "\033[0;34m   Usage:    \%s      address / prefix\n\n",program);
  fprintf(out, "1. Example:  %s     192.168.1.2/24\n\033[0m", program);
  fprintf(out, "\033[0;34m2. Example:  %s     192.168.1.2 255.255.255.0\n\n\033[0m", program);
  
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


int 
main(int argc, char **argv)
{
  
  char *address=NULL, *pre, *mask=NULL;
  data_t *ptr=NULL;
  uint32_t a,b,c,d,e,f;
  regex_t pr, re, cr;
 
  unsigned long value;
  int var1 = 0, var2 = 0;
  int var3 = 0, var4 = 0;
  int var5 = 0, prefix;

  if ((argc != 2) && (argc != 3))
    {
      usage(stderr, argv[0]);
      exit(0);
      
    }

  if(regcomp(&pr, PREFIX, REG_EXTENDED) != 0) {
    die("Error: compiling regular expression\n");
  }

  if(regcomp(&cr, CIDR, REG_EXTENDED) != 0) {
    die("Error: compiling regular expression\n");
  }
  


  if (argc == 3)
    {
      /* compile regular expression */

      if(regcomp(&re, IPEXPR, REG_EXTENDED) != 0) {
	die("Error: compiling regular expression\n");
      }
      if(regexec(&re, argv[1], 0, NULL, 0) != 0) {
	die("Error: Not an IPv4 dotted quad IP address\n");
      }

      if(regexec(&re, argv[2], 0, NULL, 0) != 0) {
	die("Error: Not an IPv4 dotted quad mask adress\n");
      }

      sscanf(argv[1], "%i.%i.%i.%i", &var1, &var2, &var3, &var4);
      if ((var1>255) || (var2>255) || (var3>255) || (var4>255)){
	die("Not an IPv4 dotted quad or CIDR notation\n");
      }

      sprintf(address, "%i.%i.%i.%i", var1, var2, var3, var4);
      
      sscanf(argv[2], "%i.%i.%i.%i", &var1, &var2, &var3, &var4);
      if ((var1>255) || (var2>255) || (var3>255) || (var4>255)){
	die("Not an IPv4 dotted quad or CIDR notation\n");
      }

      sprintf(mask, "%i.%i.%i.%i", var1, var2, var3, var4);

      prefix=maskpref(string2int(mask));
      
    }
  
  
  if (argc == 2)
    {  

      if(regexec(&cr, argv[1], 0, NULL, 0) != 0) {
	die("Error: Not an IPv4 dotted quad mask adress\n");
      }

      sscanf(argv[1], "%i.%i.%i.%i/%i", &var1, &var2, &var3, &var4, &var5);
      if ((var1>255) || (var2>255) || (var3>255) || (var4>255) || (var5>32)){
	die("Not an IPv4 dotted quad or CIDR notation\n");
      }
      
      if (strchr(argv[1], '/')==NULL) {
	die("Not an IPv4 dotted quad or CIDR notation\n");
      }

     
      address=strtok(argv[1], "/");
      pre = strtok (NULL, "/");

      if(regexec(&pr, pre, 0, NULL, 0) != 0) {
	die("Error: Not a valid prefix");
      }
      
      prefix=atoi(pre);
    }
  
  value = string2int(address);
  
  a=netmask(prefix);
  b=broadcast(value, prefix);
  c=network(value, prefix);
  d=wildcard(prefix);
  e=hostmin(value, prefix);
  f=hostmax(value, prefix);
  
  
  // I like output color :)

  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Address      :\033[0m \033[0;35m%s\033[0m\n", address);
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Netmask      :\033[0m \033[0;35m%s\033[0m\n", int2string(a));
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Wildcard     :\033[0m \033[0;35m%s\033[0m\n", int2string(d));
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Network      :\033[0m \033[0;35m%s/%d\033[0m\n", int2string(c),prefix);
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Hostmin      :\033[0m \033[0;35m%s\033[0m\n", int2string(e));
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Hostmax      :\033[0m \033[0;35m%s\033[0m\n", int2string(f));
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Broadcast    :\033[0m \033[0;35m%s\033[0m\n", int2string(b));
  printf("\033[0;34m[+]\033[0m");
  printf("\033[0;32m Hosts/Nets   :\033[0m \033[0;35m%d\033[0m\n", nethost(prefix));


  /*
  ptr=gethosts(value, prefix);
  if (ptr==NULL)
    die("Emty data\n");
  display(ptr);
  */

  return 0;
}







