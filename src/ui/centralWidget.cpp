#include "centralWidget.hpp"
#include "../api/register.hpp"
#include "../main/scriptManager.hpp"
#include "scriptCard.hpp"
#include "scriptGrid.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QSize>

#include <vector>

CentralWidget::CentralWidget(APIRegister &apiRegister, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister)
{
    setObjectName("CentralWidget");

    auto method = apiRegister_.getMethod("scripts.list");

    if (!method)
        return;

    APIResponse scriptsResponse = method({});
    auto scriptsList = scriptsResponse.result.get<std::vector<nlohmann::json>>();

    layout_ = new QVBoxLayout(this);

    scriptGrid_ = new ScriptGrid(this);
    layout_->addWidget(scriptGrid_, 1);

    for (int i = 0; i < scriptsList.size(); i++)
    {
        ScriptCard *scriptCard = new ScriptCard(scriptsList[i], apiRegister_, this);
        scriptGrid_->addScript(scriptCard);
    }
}