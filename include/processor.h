#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<vector>
#include<string>

class Processor {
    public:
    Processor() : prevTotal(0.0), prevIdle(0.0) {};
    float Utilization();
    std::vector<long> convertToLong(std::vector<std::string> values);

  // TODO: Declare any necessary private members
 private:
    long prevTotal;
    long prevIdle;
};

#endif