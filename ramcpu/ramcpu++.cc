#include <iostream>
#include <vector>
#include <algorithm>

// Memory (RAM)
int ram_size;			// how big is our ram? see main()
std::vector<int> ram;

// Processor (CPU)
struct {
  int pc;			// program counter
  int a, b;			// cpu registers
  
  struct {
    bool stopped;    // if true, cpu will stop execution
    bool eq, ne, gt, lt, ge, le; // ==, !=, >, <, >=, <=
  } flags;

  int trap[1];
  // trap[0] = divide by zero
} cpu;


// Some programs (Software)
std::vector<int> prog1 = {
  1, 14,			// load  a, 14
  2, 6,				// load  b, 6
  7,				// add   a, b
  2, 0,				// load  b, 0
  11,				// stop
};

std::vector<int> prog2 = {
  1, 1,				// load  a,1
  2, 0,				// load  b,0
  38,				// div   a,b (forces divide by zero)
  11,				// stop
};

void cmp(int a, int b);
void cpu_step();
int read_program(const char* filename, std::vector<int>& program);

int
main(int argc, char* argv[])
{
  bool debug = getenv("DEBUG") != NULL;
  
  if (argc != 2) {
    std::cout << "use : ramcpu [program_binary]\n";
    return 1;
  }

  // setup ram
  ram_size = 100;
  ram = std::vector<int>(ram_size);
  
  // setup interrupt traps
  ram[0] = 11;			// STOP
  std::fill(cpu.trap, cpu.trap + sizeof(cpu.trap), 0); // fill cpu trap jumptable

  // make the entire ram filled with the stop instrution
  if (debug)
    std::fill(begin(ram), end(ram), 11);

  // load the program
  int prog_entry = 10;
  
  std::vector<int> program;
  if (read_program(argv[1], program) != 0) return 1;
  
  std::copy(program.begin(), program.end(), ram.begin()+prog_entry); // copy the program into ram

  cpu.pc = prog_entry;	  // start executing at the first ram location
  cpu.flags.stopped = false;	// allow cpu to execute instructions
  
  while (cpu.flags.stopped == false) {
    if (debug)
      std::cout << "[debug] pc=" << cpu.pc << ": a=" << cpu.a << ", b=" << cpu.b << std::endl;
    cpu_step();
  }

  std::cout << "Execution stopped.\n\n";
  std::cout << " a = " << cpu.a << "\n";
  std::cout << " b = " << cpu.b << "\n";
  
  return 0;
}

int
read_program(const char* filename, std::vector<int>& program)
{
  FILE* fp = fopen(filename, "rb");
  
  if (!fp) {
    perror(filename);
    return 1;
  }

  fseek(fp, 0, SEEK_END);
  program.resize(ftell(fp)/sizeof(int));
  fseek(fp, 0, SEEK_SET);
  fread(&program[0], sizeof(int), program.size(), fp);
  fclose (fp);
  
  //  std::copy(begin(prog1),end(prog1),std::back_inserter(program));
  return 0;
}



// Implement the compare instruction
void
cmp(int a, int b)
{
  cpu.flags.eq = a == b;
  cpu.flags.ne = a != b;
  cpu.flags.gt = a > b;
  cpu.flags.lt = a < b;
  cpu.flags.ge = a >= b;
  cpu.flags.le = a <= b;
}

// Perform one CPU instruction
void
cpu_step()
{
  switch (ram[cpu.pc])
    {
    case 0:			// nop
      break;
      
    case 1:			// load a, value
      cpu.a = ram[++cpu.pc];
      break;
      
    case 2:			// load b, value
      cpu.b = ram[++cpu.pc];
      break;
      
    case 3:			// load a, #address
      cpu.a = ram[ram[++cpu.pc]];
      break;
      
    case 4:			// load b, #address
      cpu.b = ram[ram[++cpu.pc]];
      break;
      
    case 21:			// load a, b
      cpu.a = cpu.b;
      break;
    case 22:			// load b, a
      cpu.b = cpu.a;
      break;
      
    case 5:			// store #address, a
      ram[ram[++cpu.pc]] = cpu.a;
      break;
      
    case 6:			// store #address, b
      ram[ram[++cpu.pc]] = cpu.b;
      break;
      
    case 24:			// xchg (exchange a and b)
      {
	int tmp = cpu.a;
	cpu.a = cpu.b;
	tmp = cpu.b;
      }
      break;
      
    case 7:			// add a, b
      cpu.a = cpu.a + cpu.b;
      break;
    case 8:			// add b, a
      cpu.b = cpu.b + cpu.a;
      break;
    case 9:			// sub a, b
      cpu.a = cpu.a - cpu.b;
      break;      
    case 10:			// sub b, a
      cpu.b = cpu.b - cpu.a;
      break;
    case 36:			// mul a, b
      cpu.a = cpu.a * cpu.b;
      break;
    case 37:			// mul b, a
      cpu.b = cpu.b * cpu.a;
      break;
    case 38:			// div a, b (a=quotient, b=reminder)
      {
	if (!cpu.b)
	  {
	    cpu.pc = cpu.trap[0]; // trap[0] = divide by zero
	    return;
	  }
	
	int q = cpu.a / cpu.b;
	int r = cpu.a % cpu.b;
	cpu.a = q;
	cpu.b = r;
      }
      break;
    case 39:			// div b, a (a=quotient, b=reminder)
      {
	if (!cpu.a)
	  {
	    cpu.pc = cpu.trap[0]; // trap[0] = divide by zero
	    return;
	  }
	
	int q = cpu.b / cpu.a;
	int r = cpu.b % cpu.a;
	cpu.a = q;
	cpu.b = r;
      }
      break;
      
      
    case 11:			// stop (quit executing instructions)
      cpu.flags.stopped = true;
      return;

      // comparing
    case 14:			// cmp a, value
      cmp(cpu.a, ram[++cpu.pc]);
      break;
    case 15:			// cmp a, #address
      cmp(cpu.a, ram[ram[++cpu.pc]]);
      break;
    case 16:			// cmp a, b
      cmp(cpu.a, cpu.b);
      break;
    case 32:			// cmp b, value
      cmp(cpu.b, ram[++cpu.pc]);
      break;
    case 33:			// cmp b, #address
      cmp(cpu.b, ram[ram[++cpu.pc]]);
      break;
    case 34:			// cmd b, a
      cmp(cpu.b, cpu.a);
      break;

      // unconditional branching
    case 12:			// jmp value
      cpu.pc = ram[++cpu.pc];
      return;
    case 13:			// jmp #address
      cpu.pc = ram[ram[++cpu.pc]];
      return;

      // conditional branching
    case 17:			// jeq value (==)
      if (cpu.flags.eq) {
	cpu.pc = ram[++cpu.pc];
	return;
      }
      break;
    case 18:			// jeq #address (==)
      if (cpu.flags.eq) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;
      
    case 19:			// jne value (!=)
      if (cpu.flags.ne) {
	cpu.pc = ram[++cpu.pc];
	return;
      }
      break;
    case 20:			// jne #address (!=)
      if (cpu.flags.ne) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;

    case 23:			// jgt value (>)
      if (cpu.flags.gt) {
	cpu.pc = ram[++cpu.pc];
	return;
      }
      break;
    case 25:			// jgt #address (>)
      if (cpu.flags.gt) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;

    case 26:			// jlt value (<)
      if (cpu.flags.lt) {
	cpu.pc = ram[++cpu.pc];
	return;
      }
      break;
    case 27:			// jlt #address (<)
      if (cpu.flags.lt) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;
      
    case 28:			// jge value (>=)
      if (cpu.flags.ge) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;
    case 29:			// jge #address (>=)
      if (cpu.flags.ge) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;
      
    case 30:			// jle value (<=)
      if (cpu.flags.le) {
	cpu.pc = ram[++cpu.pc];
	return;
      }
      break;
    case 31:			// jle #address (<=)
      if (cpu.flags.le) {
	cpu.pc = ram[ram[++cpu.pc]];
	return;
      }
      break;

    case 35:			// brk (get ram size into register a)
      cpu.a = ram_size;
      break;

    case 40:			// trap num, address (set int <num> to jump to <address>
      {
	int num = ram[++cpu.pc];
	int addr = ram[++cpu.pc];
	cpu.trap[num] = addr;
      }
      break;

    case 41:			// int num (execute interrupt trap <num>
      {
	int num = ram[++cpu.pc];
	cpu.pc = cpu.trap[num];
      }
      return;
	
    } // end switch

  ++cpu.pc;			// read for the next instruction
}

