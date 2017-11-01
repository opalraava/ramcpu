#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "getopt.h"

int optind = 1;
char* optarg = NULL;

int
getopt(int argc, char* argv[], char* opt_spec)
{
  if (argv[optind] && *argv[optind] == '-')
    {
      char ch = argv[optind][1];
      
      while (*opt_spec) {
	if (*opt_spec == ch) {
	  if (argv[optind][2] != '\0') return EOF;
	  ++optind;
	  if (opt_spec[1] == ':') {
	    optarg = argv[optind++];
	  }
	  return ch;
	}
	++opt_spec;
	if (*opt_spec == ':') ++opt_spec;
      }
    }
  
  return EOF;
}
