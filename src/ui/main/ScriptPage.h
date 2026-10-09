#pragma once

#include "../../api/Register.h"
#include "../reuseables/GridWidget.h"
#include "ScriptCard.h"

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