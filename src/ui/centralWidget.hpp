#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include "../api/register.hpp"
#include "scriptGrid.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <vector>

class CentralWidget : public QWidget
{
public:
    CentralWidget(APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    ScriptGrid *scriptGrid_;
    APIRegister &apiRegister_;
    QVBoxLayout *layout_;
};

#endif