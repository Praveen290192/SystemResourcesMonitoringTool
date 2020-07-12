#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Declared new function to get the values of the required memory
float Values(string name, string location )
{
  string line;
  string key;
  string value;
  std::ifstream filestream(location);
  if(filestream.is_open())
  {
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream>>key>>value)
      {
        if(key==name){return std::stof(value);}
      }
    }
  }                                             
  return std::stof(value);
}
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string location=kProcDirectory+kMeminfoFilename;
  float Total = Values("MemTotal:",location);
  float Free  = Values("MemFree:",location);
  float Utilized = (Total-Free)/Total;
  return Utilized; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  double uptime;
  string line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open())
  {
    std::getline (stream,line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  
  return uptime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  long value;

  string line, utime, stime, cutime, cstime, temp;
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    int count = 1;
    
    while (count <= 13) {
      linestream >> temp;
      count++;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  value = std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime);
  return value;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string user,nice,system, idle, iowait,irq,softirq,steal;
  string line,key;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream>>key>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal)
      {
        if (key=="cpu")
        {
          return std::stol(user)+std::stol(nice)+std::stol(system)+std::stol(irq)+std::stol(softirq)+std::stol(steal);
        }
      }
    }
  }
  return std::stol(user)+std::stol(nice)+std::stol(system)+std::stol(irq)+std::stol(softirq)+std::stol(steal); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string user, nice, system, idle, iowait, irq, softirq, steal;
  string line, key;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream>>key>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal)
      {
        if (key=="cpu")
        {
          return std::stol(idle)+std::stol(iowait);
        }
      }
    }
  }
  return std::stol(idle)+std::stol(iowait); }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  string line;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(filestream.is_open())
  {
    std::getline(filestream,line);
    
  }
 return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{ 
  string line, key;
  string value;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open())
  {
    
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream>>key>>value)
      {
        if(key=="VmSize:"){return value;}
      }
    }
  }
  
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{   
  string line, key,value;
 
  std::ifstream filestream (kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream,line))
    {
      std::istringstream linestream(line);
      while(linestream>>key>>value)
      {
        if(key=="Uid:"){return value;}
      }
      
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  string line, temp, user, uid;
  std::ifstream filestream(kPasswordPath);
  
  if(filestream.is_open())
  {
    string UID = Uid(pid);
    while(std::getline(filestream,line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream>>user>>temp>>uid)
      {
        if(uid==UID){return user;}
      }
    }
  }
  
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  string line;
  string time;
  int count=1;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(filestream.is_open())
  {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    while(linestream >> time && count<22)
    {count++;}
  }
  return std::stol(time)/sysconf(_SC_CLK_TCK); 
}