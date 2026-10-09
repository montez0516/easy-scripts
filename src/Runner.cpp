#include "runner/engine/RunnerEngine.h"
#include "core/Paths.h"
#include "core/ipc/NamedPipeServer.h"
#include "core/ipc/NamedPipeClient.h"
#include "core/eventBus/EventBus.h"
#include "core/communication/IPCConnection.h"
#include "core/communication/Message.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <memory>

namespace fs = std::filesystem;

int main()
{
  spdlog::set_pattern("[%m/%d %T.%f %p][%^%l%$] %v");
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
#endif
  std::mutex mtx;
  std::condition_variable cv;
  bool shutdown = false;

  IPCConnection runnerPipe(std::make_unique<NamedPipeServer>("runner"));
  IPCConnection mainPipe(std::make_unique<NamedPipeClient>("easyscripts"));

  Paths paths{};
  EventBus bus;

  Engine engine{paths, bus};
  engine.initialize();

  bus.subscribe("runner.shutdown", [&mtx, &cv, &shutdown](const Message &message)
                {
    std::lock_guard<std::mutex> lock(mtx);
    shutdown = true;
    cv.notify_one(); });

  bus.subscribe("script.event", [&runnerPipe](const Message &message)
                { runnerPipe.send(message); });

  mainPipe.onMessage([&bus](const Message &message)
                     { bus.publish(message.name, message); });

  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [&shutdown]()
          { return shutdown; });

  return 0;
}