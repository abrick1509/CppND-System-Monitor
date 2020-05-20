#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string name;
  int value;
  string line;
  int mem_total{-1};
  int mem_free{-1};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if (name == "MemTotal:") {
        mem_total = value;
      } else if (name == "MemFree:") {
        mem_free = value;
      }
    }
  }
  return 1.0 - static_cast<float>(mem_free - mem_total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  long uptime{-1};
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime;
    }
  }
  return uptime / sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() {
  float result;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string name;
  int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >>
          softirq >> steal >> guest >> guest_nice;
      if (name == "cpu") {
        const auto Idle = idle + iowait;
        const auto Nonidle = user + nice + system + irq + softirq + steal;
        const auto Total = Idle + Nonidle;
        result = static_cast<float>(Nonidle / Total);
      }
    }
  }
  return result;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string name;
  int value{0};
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linstream(line);
      linstream >> name >> value;
      if (name == "processes") {
        break;
      }
    }
  }
  return value;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string name;
  int value{0};
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linstream(line);
      linstream >> name >> value;
      if (name == "procs_running") {
        break;
      }
    }
  }
  return value;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command{"-1"};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  string line;
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linstream(line);
      linstream >> command;
    }
  }
  return command;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string ram{"-1"};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line;
  string name;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linstream(line);
      linstream >> name >> ram;
      if (name == "VmSize:") break;
    }
  }
  return ram;
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid{"-1"};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line;
  string name;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linstream(line);
      linstream >> name >> uid;
      if (name == "Uid:") {
        break;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  const auto uid = Uid(pid);
  string user{"no name"};
  std::ifstream stream(kPasswordPath);
  string line;
  string x, read_uid;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> read_uid;
      if (read_uid == uid) {
        break;
      }
    }
  }
  return user;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long uptime{-1};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream.seekg(21);
      linestream >> uptime;
    }
  }
  return uptime;
}