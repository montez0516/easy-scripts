#pragma once

#include "CentralWidget.h"
#include "UIContext.h"
#include "../api/Register.h"
#include "../core/Paths.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{

public:
    MainWindow(UIContext &uiContext);

private:
    CentralWidget *centralWidget_;
    UIContext &uiContext_;
};
