#pragma once

#include "../api/register.hpp"
#include "reuseables/gridWidget.hpp"
#include "reuseables/navBar.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <vector>

class CentralWidget : public QWidget
{
public:
    CentralWidget(APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    APIRegister &apiRegister_;
    QVBoxLayout *layout_;
    GridWidget *scriptGrid_;
    NavBar *navBar_;
};