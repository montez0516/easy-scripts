#include "centralWidget.hpp"
#include "../api/register.hpp"

#include <QWidget>
#include <QSize>

CentralWidget::CentralWidget(APIRegister &apiRegister, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister)
{
    setObjectName("CentralWidget");

    auto method = apiRegister_.getMethod("scripts.list");

    if (!method)
        return;

    APIResponse scriptList = method({});
}