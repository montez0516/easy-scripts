#include "mainWindow.hpp"
#include "themeManager.hpp"
#include "centralWidget.hpp"
#include "../api/register.hpp"

#include <QMainWindow>
#include <QRect>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(APIRegister &apiRegister) : QMainWindow(nullptr), apiRegister_(apiRegister)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->geometry();
    int height = rect.height();
    int width = rect.width();

    resize(width / 2, height / 2);

    centralWidget_ = new CentralWidget(apiRegister_, this);
    setCentralWidget(centralWidget_);

    ThemeManager::loadThemeFile("dark");

    show();
}