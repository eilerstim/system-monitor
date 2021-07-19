#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<vector>
#include<string>

class Processor {
  public:
    Processor() : prevTotal(0.0), prevIdle(0.0) {};
    float Utilization();
    std::vector<long> toLong(std::vector<std::string> values);

 private:
    long prevTotal;
    long prevIdle;
};

#endif