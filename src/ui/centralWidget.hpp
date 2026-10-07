#pragma once

#include "reuseables/gridWidget.hpp"
#include "reuseables/navBar.hpp"
#include "../api/register.hpp"
#include "../core/paths.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedLayout>

#include <vector>

class CentralWidget : public QWidget
{
public:
    CentralWidget(APIRegister &apiRegister, Paths &paths, QWidget *parent = nullptr);

private:
    APIRegister &apiRegister_;
    Paths &paths_;
    QVBoxLayout *layout_;
    QStackedLayout *stackedLayout_;
    GridWidget *scriptGrid_;
    NavBar *navBar_;
    std::vector<QString> pages_ = {"script", "create", "settings"};
};