#pragma once

#include "centralWidget.hpp"
#include "../api/register.hpp"
#include "../core/paths.hpp"

#include <QMainWindow>

class MainWindow : public QMainWindow
{

public:
    MainWindow(APIRegister &apiRegister, Paths &paths);

private:
    CentralWidget *centralWidget_;
    APIRegister &apiRegister_;
    Paths &paths_;
};
