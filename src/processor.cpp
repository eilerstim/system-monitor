#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    vector<long> stats = convertToLong(LinuxParser::CpuUtilization());
    
    //calculate CPU usage

    //idle = idle +iowait
    long idle = stats.at(LinuxParser::kIdle_) + stats.at(LinuxParser::kIOwait_);

    //nonIdle = user + nice + system + irq + softirq + steal
    long nonIdle = stats.at(LinuxParser::kUser_) + stats.at(LinuxParser::kNice_) +
                    stats.at(LinuxParser::kSystem_) + stats.at(LinuxParser::kIRQ_) +
                    stats.at(LinuxParser::kSoftIRQ_) + stats.at(LinuxParser::kSteal_);
    
    //total = idle + nonIdle
    long total = idle + nonIdle;

    //calculate differences
    long totald = total - prevTotal;
    long idled = idle - prevIdle;

    //final percentage
    float cpuPercentage = (totald - idled)/totald;

    //store current values in previous ones
    prevTotal = total;
    prevIdle = idle;

    return cpuPercentage;

 }

 vector<long> Processor::convertToLong(vector<string> values) {
     vector<long> converted {};
     for (auto v:values) {
         try{
            converted.push_back(std::stol(v));
    } catch(const std::invalid_argument &arg) {
        converted.push_back(0);
        }
    }
    return converted;
 }