#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
  PidJiffies = LinuxParser::ActiveJiffies(pid);
  Jiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  C_Utilization =(float)(PidJiffies) / (float)(Jiffies);
  
  return C_Utilization;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
  long ram = std::stol(LinuxParser::Ram(pid))/1024;
  RAM = to_string(ram);
  return RAM; }

// TODO: Return the user (name) that generated this process
string Process::User() { return  LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return abs(LinuxParser::UpTime()-LinuxParser::UpTime(pid)); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return C_Utilization > a.C_Utilization; }