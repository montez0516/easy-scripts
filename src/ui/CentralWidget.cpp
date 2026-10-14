#include "CentralWidget.h"

#include "../api/Register.h"
#include "../core/Paths.h"

#include "ThemeManager.h"
#include "main/ScriptCard.h"
#include "main/CreatePage.h"
#include "main/ScriptPage.h"
#include "reuseables/GridWidget.h"
#include "reuseables/NavBar.h"
#include "reuseables/IconButton.h"
#include "reuseables/FlowLayout.h"
#include "UIContext.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QSize>
#include <QBoxLayout>
#include <QSizePolicy>
#include <Qt>

#include <vector>
#include <filesystem>

CentralWidget::CentralWidget(UIContext &uiContext, QWidget *parent) : QWidget(parent), uiContext_(uiContext)
{
    setObjectName("CentralWidget");

    auto method = uiContext.apiRegister.getMethod("scripts.list");

    if (!method)
        return;

    APIResponse scriptsResponse = method({});
    auto scriptsList = scriptsResponse.result.get<std::vector<nlohmann::json>>();

    layout_ = new QVBoxLayout(this);
    stackedLayout_ = new QStackedLayout(layout_);

    layout_->addLayout(stackedLayout_, 1);

    ScriptPage *scriptPage = new ScriptPage(uiContext_);
    CreatePage *createPage = new CreatePage(uiContext_.apiRegister);
    CreatePage *settingsPage = new CreatePage(uiContext_.apiRegister);

    stackedLayout_->addWidget(scriptPage);
    stackedLayout_->addWidget(createPage);
    stackedLayout_->addWidget(settingsPage);

    navBar_ = new NavBar(QBoxLayout::Direction::LeftToRight);
    navBar_->setObjectName("CentralNavBar");
    navBar_->setFixedHeight(65);

    layout_->addWidget(navBar_);

    std::filesystem::path iconsPath = uiContext_.paths.icons();
    QString currentTheme = ThemeManager::currentTheme();

    for (size_t i = 0; i < pages_.size(); i++)
    {
        QString page = pages_[i];
        QString fileName = page + "-" + currentTheme + ".svg";

        QString iconLocation = QString::fromStdString(
            (iconsPath / fileName.toStdString()).string());

        IconButton *button = new IconButton(page, iconLocation, navBar_);
        connect(button, &QPushButton::clicked, this, [i, this]()
                { stackedLayout_->setCurrentIndex(i); });
        navBar_->addWidget(button);
    }
}