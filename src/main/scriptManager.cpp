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

ScriptManager::ScriptManager(Paths &paths) : paths_(paths) {}

bool ScriptManager::initialize()
{

  if (!startRunner())
  {
    spdlog::critical("Failed to start runner.exe");
    return false;
  }
  loadScripts();
  return true;
}

bool ScriptManager::startRunner()
{
  if (!runnerPipe_.create())
  {
    spdlog::critical("scriptManager(startRunner): NamedPipe failed to create.");
    return false;
  }

  runnerProcess_ = std::make_unique<Process>(
      std::filesystem::absolute(paths_.runner()),
      std::vector<std::string>{});

  runnerProcess_->start();
  runnerProcess_->onFinished([this](DWORD exitCode)
                             { spdlog::critical("ScriptManager(startRunnner): runner process exited with code {}\n{}", exitCode, runnerProcess_->error()); });
  runnerProcess_->readyRead([this]()
                            { std::cout << "FROM RUNNER.EXE: " << runnerProcess_->read() << std::endl; });

  runnerPipe_.waitForConnection();

  return true;
}

void ScriptManager::loadScripts()
{
  std::filesystem::path scriptFolder = paths_.scripts();

  for (const auto &entry : std::filesystem::directory_iterator(scriptFolder))
  {
    if (entry.is_directory())
    {
      Script script(entry.path(), this);
      scripts_.push_back(script);
    }
  }
}

void ScriptManager::run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args)
{
#if defined(BUILD_DEV)
  spdlog::debug("Running script {} {}", language, scriptFile.string());
#endif
  if (runnerPipe_.isNull())
  {
    spdlog::critical("ScriptManager(run): pipe is NULL");
    return;
  }

  runnerPipe_.write(nlohmann::json({{"language", language}, {"file", scriptFile.string()}, {"args", args}}).dump());
}