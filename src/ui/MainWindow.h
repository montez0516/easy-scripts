#pragma once

#include "CentralWidget.h"
#include "../api/Register.h"
#include "../core/Paths.h"

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
