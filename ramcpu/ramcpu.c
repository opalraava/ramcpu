#include <stdio.h>
#include <stdlib.h>

#include "ramcpu.h"
#include "ramcpu_opcodes.h"

/* memory */
char* ram = NULL;
size_t ram_size = 0;

/* cpu */
typedef struct {
  size_t pc;

  char  c[CPU_NUM_CHAR_REGISTERS];
  int   r[CPU_NUM_INT_REGISTERS];
  
  struct {
    int stopped;                /* if true, cpu will stop execution */
    int eq, ne, gt, lt, ge, le; /* ==, !=, >, <, >=, <= */
  } flags;

  size_t trap_vector[CPU_SIZE_TRAP_VECTOR];
} cpu_t;

cpu_t cpu;

void init_cpu(cpu_t* cpu);
void init_ram(int _ram_size);
void load_program(char* prog_filename);
void step_cpu(cpu_t* cpu);

int
main(int argc, char* argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "use: ramcpu [program]\n");
      return 1;
    }
  
  init_ram(RAM_DEFAULT_SIZE);
  init_cpu(&cpu);
  load_program(argv[1]);

  while (cpu.flags.stopped == 0)
    step_cpu(&cpu);
  
  free(ram);
  return EXIT_SUCCESS;
}

void
init_ram(int _ram_size)
{
  if ((ram = (char*) malloc(ram_size = _ram_size)) == NULL)
    {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
}

void
init_cpu(cpu_t* cpu)
{
  int i;
  
  cpu->pc = 0;

  for (i = 0; i < CPU_NUM_CHAR_REGISTERS; i++) cpu->c[i] = '\0';
  for (i = 0; i < CPU_NUM_INT_REGISTERS; i++) cpu->r[i] = 0;
  
  cpu->flags.stopped = 0;
  cpu->flags.eq = cpu->flags.ne = cpu->flags.gt = cpu->flags.lt = cpu->flags.ge = cpu->flags.le = 0;

  for (i = 0; i < CPU_SIZE_TRAP_VECTOR; i++) cpu->trap_vector[i] = 0;
}

void load_program(char* prog_filename)
{
  FILE* fp;
  long pos;
  size_t num_bytes_read, num_bytes_in_file;

  if ((fp=fopen(prog_filename, "rb")) == NULL) {
    perror(prog_filename);
    free(ram);
    exit(EXIT_FAILURE);
  }

  if (fseek(fp, 0L, SEEK_END) != 0) {
    perror("fseek");
    fclose(fp);
    free(ram);
    exit(EXIT_FAILURE);
  }

  if ((pos = ftell(fp)) == -1L) {
    perror("ftell");
    fclose(fp);
    free(ram);
    exit(EXIT_FAILURE);
  }

  if ((num_bytes_in_file = (size_t) pos) > ram_size) {
    fprintf(stderr,
            "Error: program file bigger than available memory.\nProgram size: %lu\nMemory size: %lu\n",
            (unsigned long) num_bytes_in_file, (unsigned long) ram_size);
    fclose(fp);
    free(ram);
    exit(EXIT_FAILURE);
  }
  
  rewind(fp);
  
  if ((num_bytes_read=fread(ram, 1, num_bytes_in_file, fp)) != num_bytes_in_file) {
    perror("fread");
    fclose(fp);
    free(ram);
    exit(EXIT_FAILURE);
  }

  fclose(fp);
}

void
cmpr(cpu_t* cpu, int a, int b)
{
  cpu->flags.eq = a == b;
  cpu->flags.ne = a != b;
  cpu->flags.gt = a > b;
  cpu->flags.lt = a < b;
  cpu->flags.ge = a >= b;
  cpu->flags.le = a <= b;
}

void
cmpc(cpu_t* cpu, char a, char b)
{
  cpu->flags.eq = a == b;
  cpu->flags.ne = a != b;
  cpu->flags.gt = a > b;
  cpu->flags.lt = a < b;
  cpu->flags.ge = a >= b;
  cpu->flags.le = a <= b;
}

void
trap(cpu_t* cpu, int trap_no)
{
  fprintf(stderr, "error: trap %d triggered.\n", trap_no);
  cpu->flags.stopped = 1;
}

#define INVALID_OPCODE() do { trap(cpu, TRAP_INVALID_OPCODE); return; } while (0)

void
step_cpu(cpu_t* cpu)
{
  switch (ram[cpu->pc])
    {
    case OPCODE_NOP:
      ++cpu->pc;
      break;

    case OPCODE_STOP:
      cpu->flags.stopped = 1;
      ++cpu->pc;
      break;

    case OPCODE_LOAD:
      {
        char data_type = ram[++cpu->pc];
        char arg_type  = ram[++cpu->pc];
        int  arg_regno = (int) ram[++cpu->pc];
        
        switch (data_type)
          {
          case DATA_TYPE_INT:
            switch (arg_type)
              {
              case ARG_TYPE_REG:
                {
                  int arg = (int) ram[++cpu->pc];
                  cpu->r[arg_regno] = cpu->r[arg];
                  ++cpu->pc;
                }
                break;
              case ARG_TYPE_VALUE:
                {
                  int arg = *((int*)ram + ++cpu->pc);
                  cpu->r[arg_regno] = arg;
                  cpu->pc += sizeof(int);
                }
                break;
              case ARG_TYPE_ADDR:
                {
                  size_t addr = *((size_t*)ram + ++cpu->pc);
                  int arg = *((int*)(ram+addr));
                  cpu->r[arg_regno] = arg;
                  cpu->pc += sizeof(size_t);
                }
                break;
                
              default:
                INVALID_OPCODE(); /* invalid argument type */
              }
            break;
            
          case DATA_TYPE_CHAR:
            switch (arg_type)
              {
              case ARG_TYPE_REG:
                {
                  int arg = (int) ram[++cpu->pc];
                  cpu->c[arg_regno] = cpu->c[arg];
                  ++cpu->pc;
                }
                break;
              case ARG_TYPE_VALUE:
                {
                  char arg = ram[++cpu->pc];
                  cpu->c[arg_regno] = arg;
                  cpu->pc += sizeof(char);
                }
                break;
              case ARG_TYPE_ADDR:
                {
                  size_t addr = *((size_t*)ram + ++cpu->pc);
                  char arg = *(ram+addr);
                  cpu->c[arg_regno] = arg;
                  cpu->pc += sizeof(size_t);
                }
                break;
                
              default:
                INVALID_OPCODE(); /* invalid argument type */
              }
            break;

          default:
            INVALID_OPCODE();   /* unknown data type */
          }
      }
      break;                    /* OPCODE_LOAD */

    default:
      INVALID_OPCODE();         /* unknown opcode */
    }
}
