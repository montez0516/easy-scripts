#pragma once

#include "../../api/Register.h"

#include <QWidget>
#include <QVBoxLayout>

class CreatePage : public QWidget
{
public:
    explicit CreatePage(APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    APIRegister &apiRegister_;
    QVBoxLayout *layout_;
};