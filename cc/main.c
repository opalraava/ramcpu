#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "getopt.h"

struct commandline_options_t commandline_options;
struct commandline_libraries_t commandline_libraries;

static void init();
static void usage(char* progname);
static int process_argument(char* arg);

int
main(int argc, char* argv[])
{
  int ch, i;

  init();
  
  while ((ch = getopt(argc,argv,"cESo:gOD:I:")) != EOF) {
    switch (ch) {      
    case 'S': commandline_options.output_assembly = 1; break;
    case 'E': commandline_options.preprocessor_only = 1; break; 
    case 'c': commandline_options.stop_before_link_stage = 1; break;
    case 'o': commandline_options.link_stage_output_name = optarg; break;
    case 'g': commandline_options.output_debugging_info = 1; break;
    case 'O': commandline_options.optimize = 1; break;
    case 'D':
      commandline_options.preprocessor_define[commandline_options.preprocessor_define_count++] = optarg;
      break;
    case 'I':
      commandline_options.include_dir[commandline_options.include_dir_count++] = optarg;
      break;
    default:
      usage(argv[0]);
    }
  }

  if (optind == argc)
    usage(argv[0]);
  else
    for (i = optind; i < argc; i++) {
      if (process_argument(argv[i]) != 0)
	return 1;
    }
  
  return 0;
}



static void
usage(char* progname)
{
  printf("Use: %s [-c] [-o output] [-g] [-E] [-S] [-O] [-D name[=definition]] [-I include_path] file...\n",
	 progname);
  exit(EXIT_FAILURE);
}

static void init() {
  int i;
  
  commandline_options.preprocessor_only = 0;
  commandline_options.output_assembly = 0;
  commandline_options.stop_before_link_stage = 0;
  commandline_options.link_stage_output_name = NULL;
  commandline_options.output_debugging_info = 0;
  commandline_options.optimize = 0;

  commandline_options.preprocessor_define_count = 0;
  for (i=0;i<MAX_PREPROCESSOR_DEFINES;i++) commandline_options.preprocessor_define[i] = NULL;
  commandline_options.include_dir_count = 0;
  for (i=0;i<MAX_INCLUDE_DIRS;i++) commandline_options.include_dir[i] = NULL;

  commandline_libraries.link_libraries_count = 0;
  for (i=0;i<MAX_LINK_LIBRARIES;i++) commandline_libraries.link_libraries[i] = NULL;
}



static int process_argument(char* arg)
{
  if (arg[0] == '-' && arg[1] == 'l') {
    commandline_libraries.link_libraries[commandline_libraries.link_libraries_count++] = arg;
    return 0;
  }
  
  printf("%s\n", arg);
  return 0;
}
