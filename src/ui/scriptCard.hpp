#ifndef SCRIPT_CARD_H
#define SCRIPT_CARD_H

#include "../api/register.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>

class ScriptCard : public QWidget
{
public:
    ScriptCard(nlohmann::json scriptInfo, APIRegister &apiRegister, QWidget *parent = nullptr);

private:
    APIRegister &apiRegister_;
};

#endif