#include "ScriptPage.h"

#include "ScriptCard.h"
#include "../UIContext.h"
#include "../reuseables/FlowLayout.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"

#include <QWidget>

#include <string_view>

ScriptPage::ScriptPage(UIContext &uiContext, QWidget *parent) : QWidget(parent), uiContext_(uiContext)
{
    setObjectName("ScriptPage");

    layout_ = new FlowLayout(this, 0, 16, 16);

    auto method = uiContext_.apiRegister.getMethod("scripts.list");

    if (!method)
        return;

    APIResponse scriptResponse = method({});

    auto scriptsList = scriptResponse.result.get<std::vector<nlohmann::json>>();

    for (nlohmann::json &scriptInfo : scriptsList)
    {
        ScriptCard *scriptCard = new ScriptCard(scriptInfo);
        connect(scriptCard, &ScriptCard::runRequested, this, &ScriptPage::sendRunRequest);
        layout_->addWidget(scriptCard);
    }
}

void ScriptPage::sendRunRequest(std::string_view name)
{
    uiContext_.eventBus.publish("scripts.run", {.type = MessageType::Request, .name = "scripts.run", .id = "ScriptPage", .data = {{"name", name}}});
}