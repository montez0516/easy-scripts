#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "core/ipc/namedPipe.hpp"
#include "runner/engine/runnerEngine.hpp"
#include "core/paths.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <nlohmann/json.hpp>

#include <fstream>

namespace fs = std::filesystem;

int main()
{
  //   auto logger = spdlog::basic_logger_st("file_logger", "logs.txt", false);
  // #if defined(BUILD_DEV)
  //   logger->set_level(spdlog::level::debug);
  //   logger->flush_on(spdlog::level::debug);
  // #endif

  // spdlog::set_default_logger(logger);

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
    nlohmann::json payload = pipe.json();
    if (!payload.empty())
    {
      spdlog::debug(payload.dump());
    }
    std::string temp = "";
    engine.run(payload["language"], payload["file"], temp);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}