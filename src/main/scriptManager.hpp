#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include "script.hpp"
#include "../core/process/process.hpp"
#include "../core/ipc/namedPipe.hpp"
#include "../core/paths.hpp"
#include "../core/eventBus/eventBus.hpp"

#include <filesystem>
#include <vector>
#include <memory>
#include <string>

template<typename T>
struct ScriptEvent : Event
{
  T payload;
};

class ScriptManager
{
private:
  std::vector<Script> scripts_;
  std::vector<Process> runtimes_;
  NamedPipe mainPipe_{"easyscripts"};
  NamedPipe runnerPipe_{"runner"};
  std::unique_ptr<Process> runnerProcess_;
  Paths &paths_;
  EventBus &bus_;

  bool startRunner();
  void loadScripts();
  void registerEventListeners();
  void startServices();
public:
  ScriptManager(Paths &paths, EventBus &bus);
  bool initialize();
  void run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args);
  void stop();

  void handleEvent(const std::string &eventPayload);
};

#endif