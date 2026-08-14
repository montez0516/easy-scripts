#include "scriptManager.hpp"

#include "script.hpp"
#include "../core/process/process.hpp"
#include "../core/paths.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <memory>

ScriptManager::ScriptManager(Paths &paths) : paths_(paths)
{
}

bool ScriptManager::initialize()
{
  std::filesystem::path scriptFolder = paths_.scripts();

  if (!pipe.create())
  {
    spdlog::critical("scriptManager(): NamedPipe failed to create.");
    return false;
  }

  runner = std::make_unique<Process>(
      std::filesystem::absolute(paths_.runner()),
      std::vector<std::string>{});
  runner->start();

  pipe.waitForConnection();

  for (const auto &entry : std::filesystem::directory_iterator(scriptFolder))
  {
    if (entry.is_directory())
    {
      Script script(entry.path(), this);
      scripts.push_back(script);
    }
  }
  return true;
}

void ScriptManager::run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args)
{
#if defined(BUILD_DEV)
  spdlog::debug("Running script {} {}", language, scriptFile.string());
#endif
  if (pipe.isNull())
  {
    spdlog::critical("ScriptManager(run): pipe is NULL");
    return;
  }

  pipe.write(nlohmann::json({{"language", language}, {"file", scriptFile.string()}, {"args", args}}).dump());
}