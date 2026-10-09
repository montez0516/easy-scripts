#include "ScriptCard.h"
#include "../api/Register.h"
#include "../../core/Paths.h"
#include "IconButton.h"
#include "../ThemeManager.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

ScriptCard::ScriptCard(nlohmann::json scriptInfo, APIRegister &apiRegister, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister), scriptInfo_(scriptInfo)
{
    setObjectName("ScriptCard");

    setMinimumWidth(220);
    setMinimumHeight(150);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    std::string backgroundColor = scriptInfo.value("backgroundColor", "grey");
    std::string name = scriptInfo.value("name", "");

    std::string sheet = "background-color: " + backgroundColor + ";";

    setStyleSheet(QString::fromStdString(sheet));

    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(3, 3, 3, 3);
    layout_->setSpacing(8);

    QHBoxLayout *buttonsLayout_ = new QHBoxLayout();
    layout_->addLayout(buttonsLayout_);

    QString startIcon = QString::fromStdString((Paths::icons() / ("start-" + ThemeManager::currentTheme() + ".svg").toStdString()).string());
    IconButton *startButton = new IconButton("", startIcon);
    startButton->setFixedSize(65, 65);

    buttonsLayout_->addWidget(startButton, 0, Qt::AlignLeft);

    QString optionsIcon = QString::fromStdString((Paths::icons() / ("options-" + ThemeManager::currentTheme() + ".svg").toStdString()).string());
    IconButton *optionsButton = new IconButton("", optionsIcon);
    optionsButton->setFixedSize(65, 65);
    buttonsLayout_->addWidget(optionsButton, 0, Qt::AlignRight);

    nameLabel_ = new QLabel(QString::fromStdString(name), this);

    layout_->addWidget(nameLabel_, 0, Qt::AlignLeft | Qt::AlignBottom);
}

void ScriptCard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    std::string name = scriptInfo_.value("name", "");

    auto method = apiRegister_.getMethod("scripts.run");

    if (!method)
        return;

    APIResponse response = method({.params = name});
}