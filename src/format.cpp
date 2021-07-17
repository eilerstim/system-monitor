#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string totalTime = "00:00:00";
  long temp = 0;
  if (seconds > 0) {
    //hours
    temp = seconds / 3600;
    totalTime = valueToString(temp) + ":";
    //minutes
    temp = (seconds / 60) % 60;
    totalTime += valueToString(temp) + ":";
    //seconds
    temp = seconds % 60;
    totalTime += valueToString(temp);
  }

  return totalTime;
}

// check if calculated number is smaller than 10 and return the appropriate
// string
std::string Format::valueToString(long temp) {
  if (temp < 10)
    return "0" + std::to_string(temp);
  else
    return std::to_string(temp);
}