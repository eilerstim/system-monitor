#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    vector<long> stats = toLong(LinuxParser::CpuUtilization());

    //calculate CPU usage

    //idle = idle +iowait
    float idle = stats.at(LinuxParser::kIdle) + stats.at(LinuxParser::kIOwait);

    //nonIdle = user + nice + system + irq + softirq + steal
    float nonIdle = stats.at(LinuxParser::kUser) + stats.at(LinuxParser::kNice) +
                    stats.at(LinuxParser::kSystem) + stats.at(LinuxParser::kIrq) +
                    stats.at(LinuxParser::kSoftIrq) + stats.at(LinuxParser::kSteal) +
                    stats.at(LinuxParser::kGuest) + stats.at(LinuxParser::kGuestNice);
    
    //total = idle + nonIdle
    float total = idle + nonIdle;

    //calculate differences
    float totald = total - prevTotal;
    float idled = idle - prevIdle;

    //final percentage
    float cpuPercentage = (totald - idled)/totald;

    //store current values in previous ones
    prevTotal = total;
    prevIdle = idle;

    return cpuPercentage;

 }

 vector<long> Processor::toLong(vector<string> values) {
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