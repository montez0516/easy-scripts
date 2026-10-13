#include "core/eventBus/EventBus.h"
#include "main/ScriptManager.h"
#include "core/Paths.h"
#include "api/Register.h"
#include "main/apis/FileSystemAPI.h"
#include "main/apis/MediaAPI.h"
#include "ui/MainWindow.h"
#include "ui/UIContext.h"

#include <spdlog/spdlog.h>
#include <QApplication>
#include <QMainWindow>

#include <chrono>
#include <filesystem>
#include <thread>

int main(int argc, char **argv)
{
#if defined(BUILD_DEV)
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("APPLICATION IN DEBUG MODE SETTING LOG LEVEL TO DEBUG");
#else
  spdlog::info("APPLICATION NOT IN DEBUG MODE LOG LEVEL INFO");
#endif
  spdlog::set_pattern("[%m/%d %T.%f %p][%^%l%$] %v");

  Paths paths{};
  EventBus bus;
  APIRegister apiRegister;

  FileAPI{apiRegister};
  MediaAPI{apiRegister};

  ScriptManager manager{paths, bus, apiRegister};

  QApplication app{argc, argv};

  UIContext uiContext = {.paths = paths, .eventBus = bus, .apiRegister = apiRegister};

  MainWindow window{uiContext};

  return app.exec();
}