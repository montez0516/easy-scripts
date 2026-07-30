#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <spdlog/spdlog.h>
#include "core/ipc/namedPipe.hpp"
#include "runner/engine/runnerEngine.hpp"
#include "core/paths.hpp"

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

int main()
{
  NamedPipe pipe{};

  if(!pipe.connect(""))
    {
      spdlog::critical("runner(main): NamedPipe failed to connect.");
      return 1;
    }

  Paths paths{};

  Engine engine{paths};
  engine.initialize();

  while(true)
  {
    nlohmann::json payload = pipe.json();
    std::string temp = "";
    engine.run(payload["language"], payload["file"], temp);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}