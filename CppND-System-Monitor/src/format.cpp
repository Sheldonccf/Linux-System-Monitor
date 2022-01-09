#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    string HHMMSS;
    long int hours;
    long int mins;
    long int s;
    hours = seconds/3600;
    mins = (seconds%3600)/60;
    s = (seconds%3600)%60;
    if (hours < 10) {
      HHMMSS.append("0");
    } 
    HHMMSS.append(std::to_string(hours) + ":");
    if (mins < 10) {
      HHMMSS.append("0"); 
    }
    HHMMSS.append(std::to_string(mins) + ":");
    if (s < 10) {
      HHMMSS.append("0");
    }
    HHMMSS.append(std::to_string(s));
    return HHMMSS; }
