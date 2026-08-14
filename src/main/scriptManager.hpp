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
  std::vector<Script> scripts;
  std::vector<Process> runtimes;
  NamedPipe pipe;

  std::unique_ptr<Process> runner;
  Paths &paths_;

public:
  ScriptManager(Paths &paths);
  bool initialize();
  void
  run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args);
  void stop();
};

#endif