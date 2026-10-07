#pragma once

#include "reuseables/gridWidget.hpp"
#include "reuseables/navBar.hpp"
#include "../api/register.hpp"
#include "../core/paths.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <vector>

class CentralWidget : public QWidget
{
public:
    CentralWidget(APIRegister &apiRegister, Paths &paths, QWidget *parent = nullptr);

private:
    APIRegister &apiRegister_;
    Paths &paths_;
    QVBoxLayout *layout_;
    GridWidget *scriptGrid_;
    NavBar *navBar_;
};