#include "centralWidget.hpp"
#include "../api/register.hpp"
#include "../core/paths.hpp"
#include "themeManager.hpp"
#include "main/scriptCard.hpp"
#include "reuseables/gridWidget.hpp"
#include "reuseables/navBar.hpp"
#include "reuseables/iconButton.hpp"

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

    scriptGrid_ = new GridWidget();
    scriptGrid_->setObjectName("scriptGrid");

    layout_->addWidget(scriptGrid_, 1);

    for (int i = 0; i < scriptsList.size(); i++)
    {
        ScriptCard *scriptCard = new ScriptCard(scriptsList[i], apiRegister_, this);
        scriptGrid_->addWidget(scriptCard);
    }

    navBar_ = new NavBar(QBoxLayout::Direction::LeftToRight);
    navBar_->setObjectName("CentralNavBar");
    navBar_->setFixedHeight(65);

    layout_->addWidget(navBar_);

    std::filesystem::path iconsPath = paths_.icons();
    QString currentTheme = ThemeManager::currentTheme();

    for (const QString &page : {"script", "create", "settings"})
    {
        QString fileName = page + "-" + currentTheme + ".svg";

        QString iconLocation = QString::fromStdString(
            (iconsPath / fileName.toStdString()).string());

        IconButton *button = new IconButton(page, iconLocation, navBar_);
        connect(button, &QPushButton::clicked, this, [page]()
                { qDebug() << page; });
        navBar_->addWidget(button);
    }
}