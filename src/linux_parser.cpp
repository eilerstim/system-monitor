#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//read OS data
string LinuxParser::OperatingSystem() {
  string line{};
  string key{};
  string value{};
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
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// read kernel version
string LinuxParser::Kernel() {
  string os{};
  string kernel{}; 
  string version{};
  string line{};
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// Read and return all active process IDs
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
        try {
        int pid = stoi(filename);
        pids.push_back(pid); 
      } catch (const std::invalid_argument &arg) {
          continue;
        }
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line{};
  string key{};
  float memTotal{0};
  float memFree{0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    for(int i=0; i <=1; i++) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      if (i==0) linestream >> key >> memTotal;
      else linestream >> key >> memFree;
    }
  }
  filestream.close();
  return ((memTotal - memFree) / memTotal);
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime{0};
  string line{};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  filestream.close();
  return uptime;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> stats{};
  string line{};
  string key{};
  string user,nice,system,idle,iowait,irq,softirq,steal, guest, guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (key == "cpu") {
          stats.push_back(user);
          stats.push_back(nice);
          stats.push_back(system);
          stats.push_back(idle);
          stats.push_back(iowait);
          stats.push_back(irq);
          stats.push_back(softirq);
          stats.push_back(steal);
          stats.push_back(guest);
          stats.push_back(guest_nice);
          filestream.close();
          return stats;
        }
      }
  }
  }
  filestream.close();
  return stats;
}
// Read and return the total number of processes
int LinuxParser::TotalProcesses() {   
  string line{};
  string key{};
  string value{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "processes") {
          try {
            filestream.close();
            return std::stoi(value);
          } catch (const std::invalid_argument &arg) {
            filestream.close();
            return 0;
          }
        }
      }
    }
  }
  filestream.close();
  return 0;  }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line{};
  string key{};
  string value{};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "procs_running") {
          try {
            filestream.close();
            return std::stoi(value);
          } catch (const std::invalid_argument &arg) {
            filestream.close();
            return 0;
          }
        }
      }
    }
  }
  filestream.close();
  return 0;  }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
  }
  filestream.close();
  return line;
 }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line{};
  string key{};
  string value{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        /*changed to VmData as VmData gives the exact physical memory used as part of Physical RAM, 
        was VmSize which is actually the sum of all the virtual memory 
        and can therefore be larger than the physical RAM size */
        if (key == "VmData:") {  
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {   
  string line{};
  string key{};
  string value{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "Uid:") {
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value; }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line{};
  string user{};
  string value{};
  string temp{};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(),':',' ');
      std::istringstream linestream(line);
      while (linestream >> user >> temp >> value) {
        if (value == uid) {
          filestream.close();
          return user;
        }
      }
    }
  }
  filestream.close();
  return user;

}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line{};
  string value{};
  int temp{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      ++temp;
      if (temp == kUpTime) {
        try{
          filestream.close();
          return std::stol(value);
    } catch(const std::invalid_argument &arg) {
        filestream.close();
        return 0;
        }
        }
      }
  }
  filestream.close();
  return 0;
 }

// Read and return CPU usage of a process
vector<float> LinuxParser::CpuUtilization(int pid) {
  vector<float> stats{};
  string line{};
  string value{};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
      std::istringstream linestream(line);
      for(int i=1; i <= kUpTime; i++) {
        linestream >> value;
        if (i == kUtime || i == kStime || i == kCutime || i == kCstime ||
            i == kUpTime) {
/*
    #14 utime - CPU time spent in user code, measured in clock ticks
    #15 stime - CPU time spent in kernel code, measured in clock ticks
    #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    #22 starttime - Time when the process started, measured in clock ticks
    => divide by Hertz to get seconds
*/
              try
              {
                 stats.push_back(std::stof(value)/sysconf(_SC_CLK_TCK));
              }
              catch(const std::invalid_argument &arg)
              {
                stats.push_back(0);
              }
            }
      
  }
  }
  filestream.close();
  return stats;
}