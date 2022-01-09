#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid): Pid_(pid) {};
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
   int Pid_;
   std::string User_;
   std::string Command_;
   float CpuUtilization_;
   std::string Ram_;
   long int UpTime_; 
};

#endif
