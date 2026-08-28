#include "core/ipc/namedPipe.hpp"
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
  bus.subscribe<ScriptEvent<std::string>>([&pipe](const ScriptEvent<std::string> &event)
{
  if(event.to == "runner.exe")
  {
    spdlog::debug(
                "RUNNER BUS CALLBACK: to={} from={} payload={}",
                event.to,
                event.from,
                event.payload
            );

            if (event.to != "runner.exe")
            {
                spdlog::debug("RUNNER: ignoring event");
                return;
            }

            spdlog::debug("RUNNER: writing event to named pipe");

            pipe.write(event.payload);

            spdlog::debug("RUNNER: pipe.write returned");
  }
});
}

int main()
{
  spdlog::set_pattern("[%m/%d %T.%f %p][%^%l%$] %v");
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
#endif
  NamedPipe runnerPipe{"runner"};  
  if(!runnerPipe.create())
  {
    spdlog::critical("runner(main): runnerPipe failed to create {}", GetLastError());
    return 1;
  }
  
  NamedPipe mainPipe{"easyscripts"};
  

  if (!mainPipe.connect())
  {

    spdlog::critical("runner(main): mainPipe failed to connect {}", GetLastError());
    return 1;
  }

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
        std::string temp = "";
        spdlog::debug("RUNNING SCRIPT\n{}", jsonPayload.value<std::string>("file", ""));
        engine.run(jsonPayload["language"], jsonPayload["file"], temp);
      }
      catch (nlohmann::json_abi_v3_12_0::detail::parse_error &e)
      {
        spdlog::error("Runner(): failed to parse event payload {}", e.what());
      }
    }
  }

  return 0;
}