#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

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
#endif