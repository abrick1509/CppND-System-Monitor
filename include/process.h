#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
using std::string;
class Process {
 public:
  Process(const int pid) : pid_(pid) {}
  int Pid();                               // TODO: See src/process.cpp
  string User();                           // TODO: See src/process.cpp
  string Command();                        // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  string Ram();                            // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
  int pid_{-1};
};

#endif