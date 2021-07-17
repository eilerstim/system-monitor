#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : _pid(pid), _cpuUsage(0){}

// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long upTime = LinuxParser::UpTime();
    vector<float> stats = LinuxParser::CpuUtilization(_pid);
    if (stats.size() == 5) {
    //only if it is a valid vector
        float totaltime = stats.at(kUtime) + stats.at(kStime) + stats.at(kCutime) + stats.at(kCstime);
        float seconds = upTime - stats.at(kStarttime); //total elapsed time = uptime - starttime
        _cpuUsage = totaltime / seconds;
        return _cpuUsage;
    }
    else return 0.0;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    try{
        long ram = std::stol(LinuxParser::Ram(_pid));
        return to_string(ram/1000);                   //Divide by 1000 to get MB instead of KB
    } catch (const std::invalid_argument& arg) {
        return {};
  }
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid)/sysconf(_SC_CLK_TCK); }


