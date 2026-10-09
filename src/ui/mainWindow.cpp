#include "MainWindow.h"
#include "ThemeManager.h"
#include "CentralWidget.h"
#include "../api/Register.h"
#include "../core/Paths.h"

#include <QMainWindow>
#include <QRect>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(APIRegister &apiRegister, Paths &paths) : QMainWindow(nullptr), apiRegister_(apiRegister), paths_(paths)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->geometry();
    int height = rect.height();
    int width = rect.width();

    resize(width / 2, height / 2);

    ThemeManager::loadThemeFile(paths_, "dark");

    centralWidget_ = new CentralWidget(apiRegister_, paths_, this);
    setCentralWidget(centralWidget_);

    show();
}