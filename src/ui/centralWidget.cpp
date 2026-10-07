#include "centralWidget.hpp"
#include "../api/register.hpp"
#include "../core/paths.hpp"
#include "themeManager.hpp"
#include "main/scriptCard.hpp"
#include "reuseables/gridWidget.hpp"
#include "reuseables/navBar.hpp"
#include "reuseables/iconButton.hpp"
#include "main/createPage.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QSize>
#include <QBoxLayout>
#include <QSizePolicy>
#include <Qt>

#include <vector>
#include <filesystem>

CentralWidget::CentralWidget(APIRegister &apiRegister, Paths &paths, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister), paths_(paths)
{
    setObjectName("CentralWidget");

    auto method = apiRegister_.getMethod("scripts.list");

    if (!method)
        return;

    APIResponse scriptsResponse = method({});
    auto scriptsList = scriptsResponse.result.get<std::vector<nlohmann::json>>();

    layout_ = new QVBoxLayout(this);
    stackedLayout_ = new QStackedLayout(layout_);

    layout_->addLayout(stackedLayout_, 1);

    scriptGrid_ = new GridWidget();
    scriptGrid_->setObjectName("scriptGrid");

    stackedLayout_->addWidget(scriptGrid_);

    for (int i = 0; i < scriptsList.size(); i++)
    {
        ScriptCard *scriptCard = new ScriptCard(scriptsList[i], apiRegister_, this);
        scriptGrid_->addWidget(scriptCard);
    }

    CreatePage *createPage = new CreatePage(apiRegister_);
    CreatePage *settingsPage = new CreatePage(apiRegister_);

    stackedLayout_->addWidget(createPage);
    stackedLayout_->addWidget(settingsPage);

    navBar_ = new NavBar(QBoxLayout::Direction::LeftToRight);
    navBar_->setObjectName("CentralNavBar");
    navBar_->setFixedHeight(65);

    layout_->addWidget(navBar_);

    std::filesystem::path iconsPath = paths_.icons();
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