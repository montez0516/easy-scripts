#ifndef PROCESS_H
#define PROCESS_H

#include "../ipc/unnamedPipe.hpp"

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#include <functional>

namespace fs = std::filesystem;

class Process
{
private:
  UnnamedPipe *stdinPipe = nullptr;
  UnnamedPipe *stdoutPipe = nullptr;
  UnnamedPipe *stderrPipe = nullptr;

  fs::path exe;
  std::vector<std::string> args;
  STARTUPINFOW startupInfo{};
  PROCESS_INFORMATION processInformation{};
  std::optional<std::vector<wchar_t>> environment;
  std::wstring cwd;
  DWORD exitCode = 0;
  std::thread wait_thread;
  std::function<void(DWORD)> finishCallBack_;

  std::wstring buildCommandLine();
  void t_wait();

public:
  Process(fs::path executable, std::vector<std::string> arguments);
  bool start();
  void stop();
  DWORD wait();
  std::string read();
  void write(std::string &);
  std::string error();

  void setEnvironment(const std::map<std::wstring, std::wstring> &variables);
  void clearEnvironment();

  void setCurrentDirectory(const std::wstring &dir);
  void clearCurrentDirectory();

  bool readyRead(std::function<void()> readCallBack);
  void onFinished(std::function<void(DWORD)> finishCallBack);
};

#endif
