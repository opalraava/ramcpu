#ifndef INCLUDED_GETOPT_H
#define INCLUDED_GETOPT_H

extern int optind;
extern char* optarg;

int getopt(int argc, char* argv[], char* opt_spec);

#endif
