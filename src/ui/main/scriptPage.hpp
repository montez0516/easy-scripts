#pragma once

#include "../../api/register.hpp"
#include "../reuseables/gridWidget.hpp"
#include "scriptCard.hpp"

#include <QWidget>
#include <QVBoxLayout>

class ScriptPage : public QWidget
{
public:
    explicit ScriptPage(APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    APIRegister &apiRegister;
    GridWidget *scriptGrid;
    QVBoxLayout *layout_;
};