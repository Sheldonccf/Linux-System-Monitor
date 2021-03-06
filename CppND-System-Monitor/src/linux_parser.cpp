#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() { 
  float MemTotal, MemFree, value;
  string key;
  string unit;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal:") {
          MemTotal = value; 
        }
        if (key == "MemFree:") {
          MemFree = value;
        }
      }
    }
  }
  return (MemTotal-MemFree)/MemTotal; }

long LinuxParser::UpTime() { 
  long int timePassed;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> timePassed;
  }
  return timePassed;
 }

long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() { return 0; }

long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtils;
  string line;
  string str;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> str) {
      if (str!= "cpu") {
        cpuUtils.push_back(str);
      }
    } 
  }
  return cpuUtils;
  }

float LinuxParser::CpuUtilization(int pid) { 
  string time, line;
  const float hertz = sysconf(_SC_CLK_TCK);
  vector<string> times;
  int iterator = 1;
  std::ifstream filestream(LinuxParser::kProcDirectory + 
  to_string(pid) + LinuxParser::kStatFilename);
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
    float CpuUtilization_ = ((total_time/hertz)/seconds);
    return CpuUtilization_; 
}

int LinuxParser::TotalProcesses() { 
  string key;
  string line;
  int value;
  int totalProcess;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          totalProcess = value;         
        }
      }
    }
  }
  return totalProcess;
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key;
  string line;
  int value;
  int runningProcess;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          runningProcess = value;         
        }
      }
    }
  }
  return runningProcess;
   }

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
  std::getline(filestream, line);
  return line;
  }
  return string();
  }

string LinuxParser::Ram(int pid) { 
  string key;
  string line;
  long int value;
  string unit;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(value/1000);         
        }
      }
    }
  }
  return string();
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string key;
  string line;
  string value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;         
        }
      }
    }
  }
  return string();
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string Uid = LinuxParser::Uid(pid);
  string line;
  string key;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == Uid) {
          return value;
        }
      }
    }
  }
  return string();
  }

long LinuxParser::UpTime(int pid) { 
  long systemTime = LinuxParser::UpTime();
  long time;
  string value;
  int iterator = 1;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (filestream >> value) {
      iterator++;
      if (iterator == 22) {
        time = systemTime - std::stof(value)/sysconf(_SC_CLK_TCK);
        return time;
      }
      
    }
  }
  return 0.0;
   }
