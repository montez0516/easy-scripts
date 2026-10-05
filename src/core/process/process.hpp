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
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace fs = std::filesystem;

class Process
{

public:
  Process(fs::path executable, std::vector<std::string> arguments);
  ~Process();
  bool start();

  DWORD wait();
  bool waitFor(std::chrono::milliseconds timeout);

  bool terminate();

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

private:
  IPCConnection stdinPipe_;
  IPCConnection stdoutPipe_;
  IPCConnection stderrPipe_;

  fs::path exe_;
  std::vector<std::string> args_;

  bool captureHandles_ = false;

  STARTUPINFOW startupInfo_{};
  PROCESS_INFORMATION processInformation_{};
  std::optional<std::vector<wchar_t>> processEnvironment_;
  std::wstring cwd_;

  std::thread waitThread_;
  std::function<void(DWORD)> finishCallBack_;
  std::function<void(const Message &)> readyReadCallBack_;

  std::wstring buildCommandLine();
  void t_wait();
  bool captureProcessHandles();

  std::mutex exitMutex_;
  std::condition_variable exitCV_;
  bool exited_ = false;
  DWORD exitCode_ = STILL_ACTIVE;
};

#endif
