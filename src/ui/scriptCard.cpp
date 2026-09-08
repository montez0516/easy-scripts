#include "scriptCard.hpp"
#include "../api/register.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>

ScriptCard::ScriptCard(nlohmann::json scriptInfo, APIRegister &apiRegister, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister)
{
    setObjectName("ScriptCard");
}