#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// exception used for syntax errors
struct syntax_error {
  char* ifname;
  int line_no;
  std::string msg;
  
  syntax_error(char* ifname, int line_no, std::string msg)
    : ifname(ifname), line_no(line_no), msg(msg)
  {
  }
};

// we just want to type 'fail()' tho...
#define fail(msg) throw syntax_error(ifname, line_no, std::string(msg))

int
main(int argc,char* argv[])
{
  int as(char* ifname, std::ifstream& ifs, std::ofstream& ofs);

  // parse commandline
  
  if (argc != 4 && argv[1] != std::string("-o")) {
    std::cout << "usage: as -o [output_file] [input_file.s]\n";
    return 1;
  }

  // open them files
  
  std::ifstream ifs(argv[3]);
  if (!ifs) {
    std::cerr << "cant open input file: " << argv[3] << std::endl;
    return 1;
  }
  
  std::ofstream ofs(argv[2], std::ios::binary);
  if (!ofs) {
    std::cerr << "cant open output file: " << argv[2] << std::endl;
    return 1;
  }

  // call main function that does the work

  try {
    return as(argv[3], ifs, ofs);
  }
  catch (syntax_error ex)
    {
      std::cout << ex.ifname << ":" << ex.line_no << ": " << ex.msg << std::endl;
      return 1;
    }
}

typedef std::map<std::string, int>::iterator symtab_iter;
typedef std::multimap<std::string, int> fixup_map_t;

std::vector<int> program;
std::map<std::string, int> symtab; // symbol table
fixup_map_t fixup_map;		// fixup table
int pc = 0;			// program counter (set with .org instruction)

// this function fixes jumps to labels that are not yet defined.
void
fixup()
{
  for (auto i : fixup_map)
    {
      auto j = symtab.find(i.first);
      if (j == symtab.end()) {
	std::string msg = "forward label `" + i.first + "' not found.";
	throw std::runtime_error(msg);
      }
      program[i.second] = j->second;
    }
}


//
// main function that reads input line by line and splits each line
// into functional elements. these elements are then passed to
// process_instruction() that generates the code.
//

int
as(char* ifname, std::ifstream& ifs, std::ofstream& ofs)
{
  void process_instruction(char* ifname, int line_no, std::string label, std::string instruction,
			   std::string operand, std::string source, std::string destination);
  
  std::string line;
  static int line_no = 0;
  
  while (std::getline(ifs, line))
    {
      size_t pos;

      ++line_no;

      // kill comments and empty lines
      pos = line.find_first_of(";");
      if (pos != std::string::npos)
	line.erase(pos);

      boost::algorithm::trim(line);
      if (line.empty()) continue;


      // peel the line into parts
      std::string label;
      std::string instruction, operands;
      std::string operand, source, destination;
      
      pos = line.find_first_of(":");
      
      if (pos != std::string::npos)
	{
	  label = line.substr(0,pos);
	  if (line.begin()+pos+1 == line.end())
	      goto process_line;
	  else
	    {
	      line.erase(line.begin(), line.begin()+pos+1);
	      boost::algorithm::trim(line);
	    }
	}

      pos = line.find_first_of("\t ");
      
      if (pos != std::string::npos)
	{
	  instruction = line.substr(0,pos);
	  operands = line.substr(pos);
	  boost::algorithm::trim(operands);
	}
      else
	  instruction = line;
      
      if (!operands.empty())
	{
	  pos = operands.find_first_of(",");
	  if (pos == std::string::npos)
	      operand = operands;
	  else
	    {
	      source = operands.substr(0,pos);
	      destination = operands.substr(pos+1);
	      boost::algorithm::trim(source);
	      boost::algorithm::trim(destination);
	    }
	}
      
    process_line:
      process_instruction(ifname, line_no, label, instruction, operand, source, destination);
    }

  // fix forward declared labels
  fixup();
  
  // write program
  ofs.write((const char*)&program[0], sizeof(int)*program.size());
  
  return 0;
}





// get value from a (decimal) constant formatted like '$' <num>
int get_value(std::string val) {
  return boost::lexical_cast<int>(std::string(begin(val)+1, end(val)));  
}

// get value form a (decimal) constant formatted like '($' <num> ')'
int get_address(std::string val) {
  return boost::lexical_cast<int>(std::string(begin(val)+2, end(val)-1));
}

// convenience functions to increase program counter
inline void inc1() { pc += 1; }
inline void inc2() { pc += 2; }
inline void inc3() { pc += 3; }

// perform a jump instruction that
// jumps to a label. inserts fixups if the label
// is not yet encountered in code.
void
jump_label(int opcode, std::string label)
{
  auto i = symtab.find(label);
	
  if (i != symtab.end())
    {
      int addr = i->second;
      
      program.push_back(opcode);
      program.push_back(addr);
      inc2();
    }
  else
    {
      program.push_back(opcode);
      program.push_back(-1);
      inc2();
      
      fixup_map.insert(fixup_map_t::value_type(label, program.size()));
    }
}

void
process_instruction(char* ifname, int line_no, std::string label, std::string instruction,
		    std::string operand, std::string source, std::string destination)
{
  if (instruction == ".org")
    {
      if (operand.empty())
	fail(".org must have operand");
      
      pc = get_value(operand);
      return;
    }
  
  if (!label.empty())
    symtab[label] = pc;

  if (instruction.empty())
    return;

  if (instruction == "nop") {
    program.push_back(0);
    inc1();
  }
  
  else if (instruction == "stop") {
      program.push_back(11);
      inc1();
  }
  
  else if (instruction == "load")
    {
      if (source[0] == '$') {
	int arg = get_value(source);
	
	if (destination == "%a") {
	  program.push_back(1);
	  program.push_back(arg);
	  inc2();
	}
	else if (destination == "%b") {
	  program.push_back(2);
	  program.push_back(arg);
	  inc2();
	}
	else
	  fail("unknown destination operand");
      }
      else if (source[0] == '(') {
	int arg = get_address(source);
	if (destination == "%a") {
	  program.push_back(3);
	  program.push_back(arg);
	  inc2();
	}
	else if (destination == "%b") {
	  program.push_back(4);
	  program.push_back(arg);
	  inc2();
	}
	else
	  fail("unknown destination operand");
      }
      else if (source[0] == '%') {
	if (source == "%a" && destination == "%b") {
	  program.push_back(22);
	  inc1();
	}
	else if (source == "%b" && destination == "%a") {
	  program.push_back(21);
	  inc1();
	}
	else
	  fail("invalid operands");
      }
      else
	fail("invalid operands");
    }
  
  else if (instruction == "store")
    {
      int arg = get_address(destination);
      if (source == "%a") {
	  program.push_back(5);
	  program.push_back(arg);
	  inc2();
      }
      else if (source == "%b") {
	  program.push_back(6);
	  program.push_back(arg);
	  inc2();
      }
      else
	fail("invalid source operand");
    }
  
  else if (instruction == "add")
    {
      if (source == "%a" && destination == "%b") {
	program.push_back(8);
	inc1();
      }
      else if (source == "%b" && destination == "%a") {
	program.push_back(7);
	inc1();
      }
      else 
	fail("invalid operands");
    }
  else if (instruction == "sub")
    {
      if (source == "%a" && destination == "%b") {
	program.push_back(10);
	inc1();
      }
      else if (source == "%b" && destination == "%a") {
	program.push_back(9);
	inc1();
      }
      else
	fail("invalid operands");
    }
  else if (instruction == "mul")
    {
      if (source == "%a" && destination == "%b") {
	program.push_back(37);
	inc1();
      }
      else if (source == "%b" && destination == "%a") {
	program.push_back(36);
	inc1();
      }
      else
	fail("invalid operands");
    }
  else if (instruction == "div")
    {
      if (source == "%a" && destination == "%b") {
	program.push_back(39);
	inc1();
      }
      else if (source == "%b" && destination == "%a") {
	program.push_back(38);
	inc1();
      }
      else
	fail("invalid operands");
    }
  else if (instruction == "brk")
    {
      program.push_back(35);
      inc1();
    }
  else if (instruction == "trap")
    {
      int num = get_value(source);
      int addr = get_value(destination);
      
      program.push_back(40);
      program.push_back(num);
      program.push_back(addr);
      inc3();
    }
  else if (instruction == "int")
    {
      int num = get_value(operand);
      
      program.push_back(41);
      program.push_back(num);
      inc2();
    }
  
  // comparing
  else if (instruction == "cmp")
    {
      if (destination == "%a")
	{
	  switch(source[0])
	    {
	    case '$': {
	      int i = get_value(source);
	      program.push_back(14);
	      program.push_back(i);
	      inc2(); }
	      break;
	    case '(': {
	      int i = get_address(source);
	      program.push_back(15);
	      program.push_back(i);
	      inc2(); }
	      break;
	    case '%':		// implies %b
	      program.push_back(16);
	      break;
	      
	    default:
	      fail("invalid operands");
	    }
	}
      else if (destination == "%b")
	{
	  switch(source[0])
	    {
	    case '$': {
	      int i = get_value(source);
	      program.push_back(32);
	      program.push_back(i);
	      inc2(); }
	      break;
	    case '(': {
	      int i = get_address(source);
	      program.push_back(33);
	      program.push_back(i);
	      inc2(); }
	      break;
	    case '%':		// implies %a
	      program.push_back(34);
	      break;

	    default:
	      fail("invalid operands");
	    }
	}
    }


  else if (instruction == "jmp")
    {
      if (operand[0] == '$') {
	  int i = get_value(operand);
	  program.push_back(12);
	  program.push_back(i);
	  inc2();
      }
      else if (operand[0] == '(') {
	  int i = get_address(operand);
	  program.push_back(13);
	  program.push_back(i);
	  inc2();
      }
      else {
	jump_label(12,operand);
      }
    }

  // conditional branching
  else if (instruction == "jeq")
    {
      if (operand[0] == '$') {
	program.push_back(17);
	program.push_back(get_value(operand));
	inc2();
      }
      else if (operand[0] == '(') {
	program.push_back(18);
	program.push_back(get_address(operand));
	inc2();
      }
      else {
	jump_label(17,operand);
      }
    }
  else if (instruction == "jne")
    {
      if (operand[0] == '$') {
	program.push_back(19);
	program.push_back(get_value(operand));
	inc2();
      }
      else if (operand[0] == '(') {
	program.push_back(20);
	program.push_back(get_address(operand));
	inc2();
      }
      else {
	jump_label(19,operand);
      }
    }
  else if (instruction == "jgt")
    {
      if (operand[0] == '$') {
	program.push_back(23);
	program.push_back(get_value(operand));
	inc2();
      }
      else if (operand[0] == '(') {
	program.push_back(25);
	program.push_back(get_address(operand));
	inc2();
      }
      else {
	jump_label(23,operand);
      }
    }
  else if (instruction == "jlt")
    {
      if (operand[0] == '$') {
	program.push_back(26);
	program.push_back(get_value(operand));
	inc2();
      }
      else if (operand[0] == '(') {
	program.push_back(27);
	program.push_back(get_address(operand));
	inc2();
      }
      else {
	jump_label(26,operand);
      }
    }
  else if (instruction == "jge")
    {
      if (operand[0] == '$') {
	program.push_back(28);
	program.push_back(get_value(operand));
	inc2();
      }
      else if (operand[0] == '(') {
	program.push_back(29);
	program.push_back(get_address(operand));
	inc2();
      }
      else {
	jump_label(28,operand);
      }
    }
  else if (instruction == "jle")
    {
      if (operand[0] == '$') {
	program.push_back(30);
	program.push_back(get_value(operand));
	inc2();
      }
      else if (operand[0] == '(') {
	program.push_back(31);
	program.push_back(get_address(operand));
	inc2();
      }
      else {
	jump_label(30,operand);
      }
    }
  
  // we dont like this instruction
  else
    {
      fail("unknown instruction: " + instruction);
    }
}
