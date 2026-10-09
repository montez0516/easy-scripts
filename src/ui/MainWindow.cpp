#include "MainWindow.h"

#include "ThemeManager.h"
#include "CentralWidget.h"
#include "UIContext.h"
#include "../api/Register.h"
#include "../core/Paths.h"

#include <QMainWindow>
#include <QRect>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(UIContext &uiContext) : QMainWindow(nullptr), uiContext_(uiContext)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->geometry();
    int height = rect.height();
    int width = rect.width();

    resize(width / 2, height / 2);

    ThemeManager::loadThemeFile(uiContext_.paths, "dark");

    centralWidget_ = new CentralWidget(uiContext_, this);
    setCentralWidget(centralWidget_);

    show();
}