#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { 
    return Process::Pid_;
     }

float Process::CpuUtilization() { 
   CpuUtilization_ = LinuxParser::CpuUtilization(Pid());
   return CpuUtilization_;
}

string Process::Command() { 
    return LinuxParser::Command(Pid());
    }

string Process::Ram() { return LinuxParser::Ram(Pid());  }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& a) const {   
  return CpuUtilization_ > a.CpuUtilization_; }
