#include "scriptManager.hpp"

#include "script.hpp"
#include "events.hpp"
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
  if (!mainPipe_.open())
  {
    spdlog::critical("ScriptManager(startRunner): mainPipe failed to create {}", GetLastError());
    return false;
  }

  runnerProcess_ = std::make_unique<Process>(
      std::filesystem::absolute(paths_.runner()),
      std::vector<std::string>{});
  runnerProcess_->registerOnFinishedCallback([this](DWORD exitCode)
                                             { spdlog::critical("ScriptManager(startRunnner): runner process exited with code {}\n{}", exitCode, runnerProcess_->error()); });
  runnerProcess_->start();

  spdlog::debug("ScriptManager: waiting for runner to connect");
  mainPipe_.waitForConnection();
  spdlog::debug("ScriptManager: runner connected to main pipe");

  if (!runnerPipe_.open())
  {
    spdlog::critical("scriptManager(startRunner): runnerPipe failed to connet {}", GetLastError());
    return false;
  }

  runnerPipe_.readyRead([this]()
                        {
    std::string payload = runnerPipe_.read();
    handleEvent(payload); });
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
      Script script(entry.path(), *this, bus_);
      scripts_.push_back(script);
    }
  }
  startServices();
}

void ScriptManager::run(const std::string &language, const std::filesystem::path &scriptFile, const std::vector<std::string> &args)
{
  spdlog::debug("Running script {} {}", language, scriptFile.string());
  if (mainPipe_.isNull())
  {
    spdlog::critical("ScriptManager(run): pipe is NULL");
    return;
  }

  std::string runPayload = Events::run(scriptFile.string(), language, args);

  mainPipe_.write(runPayload);
}

#include <commdlg.h>

#include <optional>

std::optional<std::filesystem::path> openFilePicker()
{
  wchar_t fileBuffer[MAX_PATH] = {};

  OPENFILENAMEW dialog{};
  dialog.lStructSize = sizeof(dialog);
  dialog.hwndOwner = nullptr;
  dialog.lpstrFile = fileBuffer;
  dialog.nMaxFile = MAX_PATH;

  dialog.lpstrFilter =
      L"All Files\0*.*\0"
      L"Text Files\0*.txt\0"
      L"Images\0*.png;*.jpg;*.jpeg\0";

  dialog.nFilterIndex = 1;

  dialog.Flags =
      OFN_PATHMUSTEXIST |
      OFN_FILEMUSTEXIST;

  if (GetOpenFileNameW(&dialog))
    return std::filesystem::path(fileBuffer);

  return std::nullopt;
}

void ScriptManager::handleEvent(const std::string &eventPayload)
{
  try
  {
    nlohmann::json event = nlohmann::json::parse(eventPayload);
    if (event["type"] == "filepicker")
    {
      std::optional<std::filesystem::path> files = openFilePicker();

      std::string responsePayload = Events::response(event.value("from", ""), files.has_value() ? files.value() : "");

      mainPipe_.write(responsePayload);
    }
    else if (event["type"] == "msg")
    {
      spdlog::info(event["payload"]);
    }
  }
  catch (nlohmann::json_abi_v3_12_0::detail::parse_error &e)
  {
    spdlog::critical("ScriptManager(handleEvent): failed to parse event payload {}", e.what());
  }
}

void ScriptManager::registerEventListeners()
{
  bus_.subscribe<ScriptEvent<nlohmann::json>>([this](ScriptEvent<nlohmann::json> &event)
                                              {
    if(event.to != "0")
      return;

    if(event.type == "run")
    {
      std::string language = event.payload.value<std::string>("language", "");
      std::filesystem::path file = event.payload.value<std::filesystem::path>("file", "");
      std::vector<std::string> args = event.payload.value<std::vector<std::string>>("args", {});
      run(language, file, args);
    } });
}

void ScriptManager::startServices()
{
  for (Script &script : scripts_)
  {
    if (script.isService())
    {
      script.run({});
    }
  }
}