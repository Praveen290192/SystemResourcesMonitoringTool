#include <string>

#include "format.h"
#include "system.h"
#define HOUR 3600
#define MIN 60
using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long target_time) { 
  long hour   =  target_time/HOUR;
  long second = target_time%HOUR;
  long minute = second/MIN;
  second      = second%MIN;
  string st   = to_string(hour)+":"+to_string(minute)+":"+to_string(second);
  return st;
}