#pragma once

#include "ChildProcess.h"
#include "../ipc/UnnamedPipe.h"
#include "../communication/IPCConnection.h"
#include "../communication/Message.h"

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include <windows.h>
#include <functional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace fs = std::filesystem;

class ProcessHandler
{

public:
  ProcessHandler(fs::path executable, std::vector<std::string> arguments);
  ~ProcessHandler();
  bool start();

  DWORD wait();
  bool waitFor(std::chrono::milliseconds timeout);

  bool terminate();
  bool shutdown();

  void write(const Message &message);
  std::string error();

  void setEnvironment(const std::unordered_map<std::wstring, std::wstring> &variables);
  void clearEnvironment();

  void setCurrentDirectory(const std::wstring &dir);
  void clearCurrentDirectory();

  void registerReadyReadCallback(std::function<void(const Message &)> readCallBack);
  void registerOnFinishedCallback(std::function<void(DWORD)> finishCallBack);

private:
  std::unique_ptr<ChildProcess> childProcess_;

  fs::path exe_;
  std::vector<std::string> args_;

  std::optional<std::vector<wchar_t>> processEnvironment_;
  std::wstring cwd_;

  std::thread waitThread_;
  std::function<void(DWORD)> finishCallBack_;
  std::function<void(const Message &)> readyReadCallBack_;

  std::wstring buildCommandLine();

  std::string stderrBuffer_;
  std::mutex stderrMutex_;

  void t_wait();
};
