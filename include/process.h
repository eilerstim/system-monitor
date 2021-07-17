#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process (int pid); 
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  float getCpuUsage() const { return _cpuUsage;};

  // TODO: Declare any necessary private members
 private:
    int _pid;
    float _cpuUsage;
    enum States {
    kUtime = 0,
    kStime,
    kCutime,
    kCstime,
    kStarttime
  };
};

#endif