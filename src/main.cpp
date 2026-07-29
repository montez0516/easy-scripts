#include <chrono>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <thread>

#include "main/scriptManager.hpp"

int main()
{
  spdlog::set_level(spdlog::level::debug);
  ScriptManager *manager = new ScriptManager();
  manager->initialize(std::filesystem::path("../scripts"));

  while(true)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

  delete manager;
  return 0;
}