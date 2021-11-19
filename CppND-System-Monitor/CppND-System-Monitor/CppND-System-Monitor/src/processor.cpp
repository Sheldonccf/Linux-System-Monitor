#include<vector>
#include<string>
#include<iostream>

#include "processor.h"
#include "linux_parser.h"

using namespace std;
using namespace LinuxParser;
using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    vector<string> cpuUtlis = LinuxParser::CpuUtilization();
    //parse linux files to get values for current system usage by type
     
     Processor::user = std::stol(cpuUtlis[0], nullptr);
     //std::cout << Processor::user << "\n";
     Processor::nice = std::stol(cpuUtlis[1], nullptr);
     Processor::system = std::stol(cpuUtlis[2], nullptr);
     Processor::idle = std::stol(cpuUtlis[3], nullptr);
     Processor::iowait = std::stol(cpuUtlis[4], nullptr);
     Processor::irq = std::stol(cpuUtlis[5], nullptr);
     Processor::softirq = std::stol(cpuUtlis[6], nullptr);
     Processor::steal = std::stol(cpuUtlis[7], nullptr);
     Processor::guest = std::stol(cpuUtlis[8], nullptr);
     Processor::guest_nice = std::stol(cpuUtlis[9], nullptr);

    //implement the algorithm from stack overflow
     long  Idle = Processor::idle + Processor::iowait;

     long  NoIdle = Processor::user + Processor::nice + Processor::system +
     Processor::irq + Processor::softirq + Processor::steal;

     long Total = Idle + NoIdle;

     //std::cout << Total << "\n";

     long changeIdle = Idle - Processor::PrevIdle;
     long changeTotal = Total - Processor::PrevTotal;
     //std::cout << changeIdle << "\n";
     //std::cout << changeTotal << "\n";

     float Cpu_percentage = (float(changeTotal)-changeIdle)/float(changeTotal);
     //std::cout << Cpu_percentage << "\n";
    // save current total and Idle usage as Prev values
     Processor::PrevIdle = Idle;
     Processor::PrevTotal = Total;

     //std::cout << Cpu_percentage << "\n";
     return Cpu_percentage;
    }