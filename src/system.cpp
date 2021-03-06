#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  const auto pids = LinuxParser::Pids();
  // make sure we don't add too many running processes
  constexpr std::size_t noel{10};
  processes_.clear();
  processes_.reserve(noel);
  std::size_t counter{0};
  for (const auto pid : pids) {
    Process process(pid);
    // filter out processes with empty /proc/pid/cmdline
    if (process.Command() != std::string("-1")) {
      processes_.emplace_back(process);
      ++counter;
    }
    if (counter == noel) {
      break;
    }
  }
  // sort processes based on their cpu utilization
  std::sort(processes_.begin(), processes_.end());
  // reverse for decreasing order
  std::reverse(processes_.begin(), processes_.end());
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
