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
    connect(detailsPage_, DetailsPage::closeDetailsPage, this, [this]()
            { layout_->setCurrentIndex(0); });
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
        scriptLayout_->addWidget(scriptCard);
        scriptLookup_[scriptInfo.at("name")] = scriptCard;
    }
}

void ScriptPage::displayDetailsPage(const nlohmann::json &info)
{
    detailsPage_->changeInfo(info);
    layout_->setCurrentIndex(1);
}