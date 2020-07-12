#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
  
  float Active = LinuxParser::ActiveJiffies();
  float Idle = LinuxParser::IdleJiffies();
  float Total = Active+Idle;
 
   return (float)(Active)/(float)(Total);  
}