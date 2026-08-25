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

ScriptManager::ScriptManager(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus) {}

bool ScriptManager::initialize()
{

  if (!startRunner())
  {
    spdlog::critical("Failed to start runner.exe");
    return false;
  }
  registerEventListeners();
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
  runnerPipe_.readyRead([this]()
                        { handleEvent(runnerPipe_.read()); });
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
  spdlog::debug("Running script {} {}", language, scriptFile.string());
  if (runnerPipe_.isNull())
  {
    spdlog::critical("ScriptManager(run): pipe is NULL");
    return;
  }

  runnerPipe_.write(nlohmann::json({{"language", language}, {"file", scriptFile.string()}, {"args", args}}).dump());
}

void ScriptManager::handleEvent(const std::string &eventPayload)
{
  try
  {
    nlohmann::json event = nlohmann::json::parse(eventPayload);

    std::string id;
    std::string type;
    std::string payload;

    ScriptEvent e;
    e.id = id;
    e.type = type;
    e.payload = payload;
  }
  catch (nlohmann::json_abi_v3_12_0::detail::parse_error &e)
  {
    spdlog::critical("ScriptManager(handleEvent): failed to parse event payload {}", e.what());
  }
}

void ScriptManager::registerEventListeners()
{
  bus_.subscribe<ScriptEvent>([this](const ScriptEvent &event)
                              {
    if(event.id != "0")
      return;

    if(event.type == "run")
    {
      try{
        nlohmann::json payload = nlohmann::json::parse(event.payload);
        std::string language = payload.value<std::string>("language", "");
        std::filesystem::path file = payload.value<std::filesystem::path>("file", "");
        std::vector<std::string> args = payload.value<std::vector<std::string>>("args", {});
        run(language, file, args);
      } 
      catch(nlohmann::json_abi_v3_12_0::detail::parse_error &e)
      {
        spdlog::error("ScriptManager(registerEventListeners): failed to parse ScriptEvent payload {}\n{}", event.payload, e);
      }
    } });
}