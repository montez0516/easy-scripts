#include "core/ipc/namedPipe.hpp"
#include "runner/engine/runnerEngine.hpp"
#include "core/paths.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

int main()
{
  //   auto logger = spdlog::basic_logger_st("file_logger", "logs.txt", false);
  // #if defined(BUILD_DEV)
  //   logger->set_level(spdlog::level::debug);
  //   logger->flush_on(spdlog::level::debug);
  // #endif

  // spdlog::set_default_logger(logger);

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

  Engine engine{paths};
  engine.initialize();

  while (true)
  {
    std::string payload = pipe.read();
    spdlog::debug("_RUNNER PAYLOAD {}", payload);
    if (!payload.empty())
    {
      try
      {
        nlohmann::json jsonPayload = nlohmann::json::parse(payload);
        std::string temp = "";
        spdlog::debug("_RUNNER RUNNING SCRIPT {}", jsonPayload.value<std::string>("file", ""));
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