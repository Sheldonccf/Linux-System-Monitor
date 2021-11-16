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

// TODO: Return this process's ID
int Process::Pid() { 
    return Process::Pid_;
     }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  string time, line;
  const float hertz = sysconf(_SC_CLK_TCK);
  vector<string> times;
  int iterator = 1;
  std::ifstream filestream(LinuxParser::kProcDirectory + 
  to_string(Pid_) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while((linestream >> time) && (iterator <= 22)) {
      iterator ++;
      times.push_back(time);
      }
    }
      
    float total_time = stof(times[13]) + stof(times[14])
        +stof(times[15]) +stof(times[16]);
    float uptime = LinuxParser::UpTime();
    float starttime = stof(times[21]);            
    float seconds = uptime - starttime/hertz;       
    CpuUtilization_ = ((total_time/hertz)/seconds);
    return CpuUtilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Pid());
    }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid());  }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  
  return CpuUtilization_ > a.CpuUtilization_; }