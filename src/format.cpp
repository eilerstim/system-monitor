#include <string>

#include "format.h"

using std::string;

// check if calculated number is smaller than 10 and return the appropriate
// string
string formatSMH(int val){
  string strval{""};
  if (val < 10){
    strval = "0" + std::to_string(val);
  }
  else{
    strval = std::to_string(val);
  }
  return strval;
}

string Format::ElapsedTime(long seconds) {
  int hours{0}, minutes{0};
  string ftime{""};
  minutes = seconds / 60;
  hours = minutes / 60;

  ftime = formatSMH(hours)+":"+formatSMH(int(minutes%60))+":"+formatSMH(int(seconds%60));
  return ftime;
}