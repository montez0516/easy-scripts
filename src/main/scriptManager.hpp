#pragma once

#include "scriptInfo.hpp"
#include "../core/process/process.hpp"
#include "../core/ipc/namedPipeServer.hpp"
#include "../core/ipc/namedPipeClient.hpp"
#include "../core/paths.hpp"
#include "../core/eventBus/eventBus.hpp"
#include "../api/register.hpp"
#include "../core/communication/ipcConnection.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <vector>
#include <memory>
#include <string>
#include <optional>

class ScriptManager
{

public:
  ScriptManager(Paths &paths, EventBus &bus, APIRegister &apiRegister);
  ~ScriptManager();
  void run(std::string scriptName, const std::vector<std::string> &args);
  void stop();

  APIResponse list(APIRequest request);

  void handleEvent(const std::string &eventPayload);

private:
  std::vector<ScriptInfo> scripts_;
  std::vector<Process> runtimes_;
  IPCConnection mainPipe_;
  IPCConnection runnerPipe_;
  std::unique_ptr<Process> runnerProcess_;
  Paths &paths_;
  EventBus &bus_;
  APIRegister &apiRegister_;

  bool startRunner();
  void loadScripts();
  void registerEventListeners();
  void startServices();

  ScriptInfo &getScript(std::string id);
  bool hasScript(std::string id);
};
