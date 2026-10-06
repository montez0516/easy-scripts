#pragma once

#include "centralWidget.hpp"
#include "../api/register.hpp"

#include <QMainWindow>

class MainWindow : public QMainWindow
{

public:
    MainWindow(APIRegister &apiRegister_);

private:
    CentralWidget *centralWidget_;
    APIRegister &apiRegister_;
};
