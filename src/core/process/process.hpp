#ifndef PROCESS_H
#define PROCESS_H

#include "../ipc/unnamedPipe.hpp"
#include "../communication/ipcConnection.hpp"
#include "../communication/message.hpp"

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
  IPCConnection stdinPipe_;
  IPCConnection stdoutPipe_;
  IPCConnection stderrPipe_;

  fs::path exe_;
  std::vector<std::string> args_;

  STARTUPINFOW startupInfo_{};
  PROCESS_INFORMATION processInformation_{};
  std::optional<std::vector<wchar_t>> processEnvironment_;
  std::wstring cwd_;
  bool captureHandles_ = false;

  DWORD exitCode_ = 0;

  std::thread waitThread_;
  std::function<void(DWORD)> finishCallBack_;
  std::function<void(const Message &)> readyReadCallBack_;

  std::wstring buildCommandLine();
  void t_wait();

  bool captureProcessHandles();

public:
  Process(fs::path executable, std::vector<std::string> arguments);
  ~Process();
  bool start();
  void stop();
  DWORD wait();

  std::string read();
  void write(const Message &message);
  std::string error();

  void setEnvironment(const std::map<std::wstring, std::wstring> &variables);
  void clearEnvironment();

  void setCurrentDirectory(const std::wstring &dir);
  void clearCurrentDirectory();

  void setCaptureHandles(bool value);

  void registerReadyReadCallback(std::function<void(const Message &)> readCallBack);
  void registerOnFinishedCallback(std::function<void(DWORD)> finishCallBack);
};

#endif
