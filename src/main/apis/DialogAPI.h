#pragma once

#include "../../api/Register.h"

#include <QWidget>

class DialogAPI
{

public:
    explicit DialogAPI(APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    APIResponse showInput(const APIRequest &request);
    APIResponse showSelect(const APIRequest &request);

    QWidget *parent_;
};