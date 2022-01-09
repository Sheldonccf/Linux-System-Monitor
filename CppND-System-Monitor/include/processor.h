#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  
  
 private:
  long int user;
  long int nice;
  long int system;
  long int idle;
  long int iowait;
  long int irq;
  long int softirq;
  long int steal;
  long int guest;
  long int guest_nice;
  long int PrevTotal;
  long int PrevIdle;
};

#endif
