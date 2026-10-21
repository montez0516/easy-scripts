#include "ScriptPage.h"

#include "ScriptCard.h"
#include "../UIContext.h"
#include "../reuseables/FlowLayout.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"
#include "DetailsPage.h"

#include <QWidget>
#include <QStackedLayout>

#include <string_view>

ScriptPage::ScriptPage(UIContext &uiContext, QWidget *parent) : QWidget(parent), uiContext_(uiContext)
{
    setObjectName("ScriptPage");

    layout_ = new QStackedLayout(this);

    scriptPage_ = new QWidget();
    layout_->addWidget(scriptPage_);

    scriptLayout_ = new FlowLayout(scriptPage_, 0, 16, 16);

    detailsPage_ = new DetailsPage(uiContext_.eventBus);
    connect(detailsPage_, &DetailsPage::closeDetailsPage, this, [this]()
            {
                layout_->setCurrentIndex(0);
                detailsPage_->deactivate();
            });
    connect(detailsPage_, &DetailsPage::requestScriptStart, this, &ScriptPage::sendStartRequest);
    connect(detailsPage_, &DetailsPage::requestScriptStop, this, &ScriptPage::sendStopRequest);

    layout_->addWidget(detailsPage_);

    auto method = uiContext_.apiRegister.getMethod("scripts.list");

    if (!method)
        return;

    APIResponse scriptResponse = method({});

    auto scriptsList = scriptResponse.result.get<std::vector<nlohmann::json>>();

    for (nlohmann::json &scriptInfo : scriptsList)
    {
        ScriptCard *scriptCard = new ScriptCard(scriptInfo, uiContext_.eventBus, this);
        connect(scriptCard, &ScriptCard::requestDetailsPage, this, &ScriptPage::displayDetailsPage);
        connect(scriptCard, &ScriptCard::requestScriptStart, this, &ScriptPage::sendStartRequest);
        connect(scriptCard, &ScriptCard::requestScriptStop, this, &ScriptPage::sendStopRequest);

        scriptLayout_->addWidget(scriptCard);
        scriptLookup_[scriptInfo.at("name")] = scriptCard;
    }
}

void ScriptPage::displayDetailsPage(const nlohmann::json &info)
{
    detailsPage_->changeInfo(info);
    layout_->setCurrentIndex(1);
    detailsPage_->activate();
}

void ScriptPage::sendStartRequest(const std::string &id, std::vector<std::string> args)
{
    uiContext_.eventBus.publish("scripts.run", {.type = MessageType::Request, .id = id, .data = {{"args", args}}});
}

void ScriptPage::sendStopRequest(const std::string &id)
{
    uiContext_.eventBus.publish("scripts.stop", {.type = MessageType::Request, .id = id});
}
