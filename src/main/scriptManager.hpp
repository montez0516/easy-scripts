#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include "script.hpp"
#include "../core/process/process.hpp"
#include "../core/ipc/namedPipe.hpp"
#include "../core/paths.hpp"

#include <filesystem>
#include <vector>
#include <memory>

class ScriptManager
{
private:
  std::vector<Script> scripts_;
  std::vector<Process> runtimes_;
  NamedPipe runnerPipe_;

  std::unique_ptr<Process> runnerProcess_;
  Paths &paths_;
  bool startRunner();
  void loadScripts();

public:
  ScriptManager(Paths &paths);
  bool initialize();
  void
  run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args);
  void stop();
};

#endif