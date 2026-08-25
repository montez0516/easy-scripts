#include "core/eventBus/eventBus.hpp"
#include "main/scriptManager.hpp"
#include "core/paths.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <thread>


int main()
{
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
#endif

  Paths paths{};

  EventBus bus;

  ScriptManager manager{paths};
  manager.initialize();

  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}