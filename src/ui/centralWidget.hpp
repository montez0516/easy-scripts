#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include "../api/register.hpp"

#include <QWidget>
#include <QGridLayout>

class CentralWidget : public QWidget
{
public:
    CentralWidget(APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    QGridLayout *gridLayout_;
    APIRegister &apiRegister_;
};

#endif