#include "core/eventBus/eventBus.hpp"
#include "main/scriptManager.hpp"
#include "core/paths.hpp"
#include "api/register.hpp"
#include "main/apis/fileAPI.hpp"
#include "main/apis/mediaAPI.hpp"
#include "ui/mainWindow.hpp"

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

  MainWindow window{apiRegister};

  return app.exec();
}