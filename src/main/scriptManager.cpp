#include "scriptManager.hpp"

#include "script.hpp"
#include "../core/process/process.hpp"
#include "../core/paths.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>


ScriptManager::ScriptManager(Paths &paths) : paths_(paths)
 { 
  pipe = new NamedPipe();
 }

bool ScriptManager::initialize()
{
  std::filesystem::path scriptFolder = paths_.scripts();

  if(!pipe->create())
    {
      spdlog::critical("scriptManager(): NamedPipe failed to create.");
      delete pipe;
      pipe = nullptr;
      return false;
    }

  runner = new Process(std::filesystem::absolute(paths_.runner()), {});
  runner->start();

  for(const auto &entry : std::filesystem::directory_iterator(scriptFolder))
    {
      if(entry.is_directory())
        {
          Script script(entry.path(), this);
          scripts.push_back(script);
        }
    }
  return true;
}

void ScriptManager::run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args)
{
  if(pipe->isNull())
    {
      spdlog::critical("ScriptManager(run): pipe is NULL");
      return;
    }

  pipe->waitForConnection();

  pipe->write(nlohmann::json({ { "language", language }, { "file", scriptFile.string() }, { "args", args } }).dump());

}