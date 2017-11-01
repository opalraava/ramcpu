#ifndef INCLUDED_MAIN_H
#define INCLUDED_MAIN_H

#define MAX_PREPROCESSOR_DEFINES 8
#define MAX_INCLUDE_DIRS 16
#define MAX_LINK_LIBRARIES 8

struct commandline_options_t {
  int preprocessor_only;	/* -E */
  int output_assembly;		/* -S */
  int stop_before_link_stage;	/* -c */
  char* link_stage_output_name;	/* -o arg */
  int output_debugging_info;	/* -g */
  int optimize;			/* -O */
  
  /* preprocessor: lists of include dirs and preprocessor defines */
  char* preprocessor_define[MAX_PREPROCESSOR_DEFINES]; /* -D arg */
  int preprocessor_define_count;
  char* include_dir[MAX_INCLUDE_DIRS];		       /* -I arg */
  int include_dir_count;
};

struct commandline_libraries_t {
  char* link_libraries[MAX_LINK_LIBRARIES]; /* -lc */
  int link_libraries_count;
};

extern struct commandline_options_t commandline_options;
extern struct commandline_libraries_t commandline_libraries;

#endif
