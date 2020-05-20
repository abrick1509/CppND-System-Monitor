#include "format.h"

#include <iostream>
#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(const long seconds) {
  string result;
  const int sec = seconds % 60;
  const int total_minutes = seconds / 60;
  const int min = total_minutes % 60;
  const int hours = total_minutes / 60;
  result = std::to_string(hours) + ":" + std::to_string(min) + ":" +
           std::to_string(sec);
  return result;
}
