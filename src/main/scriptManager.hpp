#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include <filesystem>
#include <vector>

#include "../core/process/process.hpp"
#include "script.hpp"
#include "../core/ipc/namedPipe.hpp"

class ScriptManager
{
private:
  std::vector<Script> scripts;
  std::vector<Process> runtimes;
  NamedPipe *pipe = nullptr;

  Process *runner = nullptr;

public:
  ScriptManager();
  bool initialize(const std::filesystem::path &ScriptFolder);
  void
  run(const std::string &language, const std::filesystem::path &scriptFile,
      const std::vector<std::string> &args);
  void stop();
};

#endif