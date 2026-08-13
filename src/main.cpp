#include <chrono>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <thread>

#include "main/scriptManager.hpp"
#include "core/paths.hpp"

int main()
{
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
#endif

  Paths paths{};

  ScriptManager manager{paths};
  manager.initialize();

  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}