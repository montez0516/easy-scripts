#include "core/ipc/namedPipe.hpp"
#include "runner/engine/runnerEngine.hpp"
#include "core/paths.hpp"
#include "core/eventBus/eventBus.hpp"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main()
{
  spdlog::set_pattern("[%m/%d %T.%f %p][%^%l%$] %v");
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
#endif
  NamedPipe pipe{};

  if (!pipe.connect())
  {

    spdlog::critical("runner(main): NamedPipe failed to connect.");
    return 1;
  }

  Paths paths{};
  EventBus bus;

  Engine engine{paths, bus};
  engine.initialize();

  while (true)
  {
    std::string payload = pipe.read();
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
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}