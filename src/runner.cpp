#include "core/ipc/namedPipeServer.hpp"
#include "core/ipc/namedPipeClient.hpp"
#include "runner/engine/runnerEngine.hpp"
#include "core/paths.hpp"
#include "core/eventBus/eventBus.hpp"
#include "main/scriptManager.hpp"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void registerEventListeners(EventBus &bus, NamedPipe &pipe)
{
  bus.subscribe<ScriptEvent<std::string>>([&pipe](ScriptEvent<std::string> &event)
                                          {
  if(event.to == "runner.exe")
  {

            if (event.to != "runner.exe")
            {
                return;
            }

            try{
              nlohmann::json jsonPayload = nlohmann::json::parse(event.payload);
              jsonPayload["from"] = event.from;
              pipe.write(jsonPayload.dump());

            }
            catch(nlohmann::json_abi_v3_12_0::detail::parse_error &e)
            {
              spdlog::error("RUNNER(registerEventListeners): failed to parse payload {}\n{}", event.payload, e.what());
            }

  } });
}

int main()
{
  spdlog::set_pattern("[%m/%d %T.%f %p][%^%l%$] %v");
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
#endif
  NamedPipeServer runnerPipe{"runner"};
  if (!runnerPipe.open())
  {
    spdlog::critical("runner(main): runnerPipe failed to create {}", GetLastError());
    return 1;
  }

  NamedPipeClient mainPipe{"easyscripts"};

  if (!mainPipe.open())
  {

    spdlog::critical("runner(main): mainPipe failed to connect {}", GetLastError());
    return 1;
  }

  runnerPipe.waitForConnection();

  Paths paths{};
  EventBus bus;

  Engine engine{paths, bus};
  engine.initialize();

  registerEventListeners(bus, runnerPipe);

  while (true)
  {
    std::string payload = mainPipe.read();
    if (!payload.empty())
    {
      spdlog::debug("SCRIPT PAYLOAD\n{}", payload);
      try
      {
        nlohmann::json jsonPayload = nlohmann::json::parse(payload);

        std::string type = jsonPayload.value<std::string>("type", "");

        if (type == "run")
        {
          std::string temp = "";
          spdlog::debug("RUNNING SCRIPT\n{}", jsonPayload.value<std::string>("file", ""));
          engine.run(jsonPayload["payload"]["language"], jsonPayload["payload"]["file"], temp);
        }
        else if (type == "response")
        {
          ScriptEvent<std::string> event;
          event.to = jsonPayload["to"];
          event.payload = jsonPayload["payload"];
          bus.publish(event);
        }
      }
      catch (nlohmann::json_abi_v3_12_0::detail::parse_error &e)
      {
        spdlog::error("Runner(): failed to parse event payload {}", e.what());
      }
    }
    else
      return 1;
  }

  return 0;
}