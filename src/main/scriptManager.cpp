#include "scriptManager.hpp"

#include "scriptInfo.hpp"
#include "events.hpp"
#include "../core/process/process.hpp"
#include "../core/paths.hpp"
#include "../api/register.hpp"

#include "../core/communication/message.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>

ScriptManager::ScriptManager(Paths &paths, EventBus &bus, APIRegister &apiRegister) : paths_(paths), bus_(bus), apiRegister_(apiRegister)
{
  if (!startRunner())
  {
    spdlog::critical("Failed to start runner.exe");
    return;
  }
  registerEventListeners();
  loadScripts();

  apiRegister_.registerMethod("scripts.list", [this](APIRequest request)
                              { return list(request); });
  apiRegister_.registerMethod("scripts.run", [this](APIRequest request)
                              {
                                APIResponse response;
                                response.status = true;

                                this->run(request.params, {});

                                return response; });
}

ScriptManager::~ScriptManager()
{
  mainPipe_.send(
      {
          .type = MessageType::Event,
          .name = "runner.shutdown",
          .id = "scriptManager",
      });
}

bool ScriptManager::startRunner()
{
  mainPipe_ = IPCConnection(std::make_unique<NamedPipeServer>("easyscripts"));

  runnerProcess_ = std::make_unique<Process>(
      std::filesystem::absolute(paths_.runner()),
      std::vector<std::string>{});
  runnerProcess_->registerOnFinishedCallback([this](DWORD exitCode)
                                             { spdlog::critical("ScriptManager(startRunnner): runner process exited with code {}\n{}", exitCode, runnerProcess_->error()); });
  runnerProcess_->start();

  if (!mainPipe_.waitForConnection())
  {
    spdlog::error("ScriptManager(startRunner): Failed to recieve connection from runner {}", GetLastError());
  }

  runnerPipe_ = IPCConnection(std::make_unique<NamedPipeClient>("runner"));
  runnerPipe_.onMessage([](const Message &message)
                        { spdlog::debug("ScriptManager(startRunner): {} {} {}", message.id, message.name, message.data.dump()); });
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

  mainPipe_.send({.type = MessageType::Event,
                  .name = "script.run",
                  .id = scriptInfo.get("name"),
                  .data = {
                      {"file", file},
                      {"language", language}}});
  scriptInfo.running = true;
  spdlog::debug("ScriptManager(run): Ran script", language, file);
}

void ScriptManager::handleEvent(const std::string &eventPayload)
{
  spdlog::debug("ScriptManager(handleEvent): {}", eventPayload);
  try
  {
    nlohmann::json event = nlohmann::json::parse(eventPayload);

    std::string type = event["type"];

    if (type != "finished")
    {

      auto method = apiRegister_.getMethod(type);

      if (!method)
      {
        spdlog::error("ScriptManager(handleEvent): method with type {} does not exist", type);
        mainPipe_.send({.type = MessageType::Response,
                        .name = "script.response",
                        .id = event["from"],
                        .data = ""});
        return;
      }

      APIRequest req;
      req.params = event.value("payload", "");

      APIResponse res = method(req);

      mainPipe_.send({.type = MessageType::Response,
                      .name = "script.response",
                      .id = event["from"],
                      .data = res.result});
    }
    else
    {
      std::string scriptName = event["from"];

      if (!hasScript(scriptName))
        return;
      ScriptInfo &scriptInfo = getScript(event["from"]);
      scriptInfo.running = false;
      spdlog::debug("ScriptManager(handleEvent): script no longer running {}", scriptInfo.get("name"));
    }
  }
  catch (nlohmann::json::exception &e)
  {
    spdlog::critical("ScriptManager(handleEvent): failed to parse event payload {}", e.what(), eventPayload);
  }
}

void ScriptManager::registerEventListeners()
{
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