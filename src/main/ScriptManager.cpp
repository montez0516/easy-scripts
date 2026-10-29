#include "ScriptManager.h"

#include "ScriptInfo.h"
#include "../core/process/ProcessHandler.h"
#include "../core/Paths.h"
#include "../api/Register.h"
#include "../core/eventBus/EventBus.h"
#include "../core/communication/Message.h"
#include "../core/ipc/NamedPipeServer.h"
#include "../core/ipc/NamedPipeClient.h"
#include "../core/utils/windows/WindowsUtils.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <chrono>

ScriptManager::ScriptManager(Paths &paths, EventBus &bus, APIRegister &apiRegister) : paths_(paths), bus_(bus), apiRegister_(apiRegister)
{
  if (!startRunner())
  {
    spdlog::critical("Failed to start runner.exe");
    return;
  }
  registerEventListeners();
  loadScripts();
}

ScriptManager::~ScriptManager()
{
  mainPipe_->send({.type = "runner.shutdown"});

  if (!runnerProcess_->waitFor(std::chrono::milliseconds(5000)))
  {
    runnerProcess_->terminate();
  }
}

bool ScriptManager::startRunner()
{
  mainPipe_ = std::make_unique<IPCConnection>(std::make_unique<NamedPipeServer>("easyscripts"));

  runnerProcess_ = std::make_unique<Process>(
      std::filesystem::absolute(paths_.runner()),
      std::vector<std::string>{});
  runnerProcess_->registerOnFinishedCallback([this](DWORD exitCode)
                                             {
                                               spdlog::critical("ScriptManager(startRunnner): runner process exited with code {}\n{}", exitCode, runnerProcess_->error());
                                               exit(exitCode); });
  runnerProcess_->start();

  if (!mainPipe_->waitForConnection())
  {
    spdlog::error("ScriptManager(startRunner): Failed to recieve connection from runner {}", Utils::Windows::GetError());
  }

  runnerPipe_ = std::make_unique<IPCConnection>(std::make_unique<NamedPipeClient>("runner"));
  runnerPipe_->onMessage([this](const Message &message)
                         { bus_.publish(message.type, message); });
  bus_.subscribe("files.filepicker", [this](const Message &message)
                 {
                  spdlog::debug("testing filepicker {}({})::{}", message.scriptName, message.promiseID, message.type);
                   auto method = apiRegister_.getMethod("files.filepicker");

                   if (!method)
                     return;

                   APIResponse response = method({});

                   mainPipe_->send({.type = "runtime.response", .scriptName = message.scriptName, .promiseID = message.promiseID, .data = response.result}); });
  return true;
}

void ScriptManager::loadScripts()
{
  spdlog::debug("Loading Scripts");
  std::filesystem::path scriptFolder = paths_.scripts();

  for (const auto &entry : std::filesystem::directory_iterator(scriptFolder))
  {
    spdlog::debug(entry.path().string());
    if (entry.is_directory())
    {
      ScriptInfo script(entry.path());
      scripts_.push_back(script);
    }
  }
  startServices();
}

void ScriptManager::run(std::string scriptName, const std::vector<std::string> &args)
{
  if (!hasScript(scriptName))
    return;
  ScriptInfo &scriptInfo = getScript(scriptName);

  if (scriptInfo.running)
  {
    spdlog::debug("ScriptManager(run): script already running {}", scriptInfo.get("name"));
    return;
  }

  std::string file = scriptInfo.get("file");
  std::string language = scriptInfo.get("language");

  spdlog::debug("Running script {} {}", language, file);

  mainPipe_->send({.type = "scripts.run",
                   .scriptName = scriptInfo.get("name"),
                   .data = {
                       {"file", file},
                       {"language", language}}});
  spdlog::debug("ScriptManager(run): Ran script", language, file);
}

void ScriptManager::registerEventListeners()
{
  apiRegister_.registerMethod("scripts.list", [this](APIRequest request)
                              { return list(request); });
  bus_.subscribe("scripts.run", [this](const Message &message)
                 {
                   std::string name = message.scriptName;
                   //  std::vector<std::string> args = message.data.value<std::vector<std::string>>("args", {});
                   run(name, {}); });
  bus_.subscribe("scripts.stop", [this](const Message &message)
                 { mainPipe_->send(message); });
  bus_.subscribe("scripts.state", [this](const Message &message)
                 {
                   std::string state = message.data.at("state");
                   ScriptInfo &scriptInfo = getScript(message.scriptName);

                   if (state == "scripts.started")
                   {
                     scriptInfo.running = true;
                   }
                   else if (state == "scripts.finished" || state == "scripts.error")
                   {
                     scriptInfo.running = false;
                   } });
}

APIResponse ScriptManager::list(APIRequest request)
{
  nlohmann::json scriptsInfo = nlohmann::json::array();
  for (ScriptInfo &script : scripts_)
  {
    scriptsInfo.push_back(script.getAll());
  }

  APIResponse response;
  response.status = true;
  response.result = scriptsInfo;
  return response;
}

void ScriptManager::startServices()
{
  for (ScriptInfo &script : scripts_)
  {
    if (script.get("type") == "service")
    {
      run(script.get("name"), {});
    }
  }
}

ScriptInfo &ScriptManager::getScript(std::string id)
{
  for (auto &script : scripts_)
  {
    if (script.get("name") == id)
    {
      return script;
    }
  }
}

bool ScriptManager::hasScript(std::string id)
{
  for (auto &script : scripts_)
  {
    if (script.get("name") == id)
    {
      return true;
    }
  }
  return false;
}