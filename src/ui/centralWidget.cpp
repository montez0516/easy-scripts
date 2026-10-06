#include "centralWidget.hpp"
#include "../api/register.hpp"
#include "../main/scriptManager.hpp"
#include "scripts/scriptCard.hpp"
#include "reuseables/gridWidget.hpp"
#include "reuseables/navBar.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QSize>
#include <QBoxLayout>
#include <QSizePolicy>

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

    scriptGrid_ = new GridWidget();
    scriptGrid_->setObjectName("scriptGrid");

    layout_->addWidget(scriptGrid_, 9);

    for (int i = 0; i < scriptsList.size(); i++)
    {
        ScriptCard *scriptCard = new ScriptCard(scriptsList[i], apiRegister_, this);
        scriptGrid_->addWidget(scriptCard);
    }

    navBar_ = new NavBar(QBoxLayout::Direction::LeftToRight);
    navBar_->setObjectName("centralBar");
    layout_->addWidget(navBar_, 1);
}