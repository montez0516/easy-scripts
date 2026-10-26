#include "ScriptCard.h"
#include "IconButton.h"
#include "../../core/Paths.h"
#include "../utils/ThemeManager.h"
#include "../utils/IconManager.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

ScriptCard::ScriptCard(nlohmann::json scriptInfo, EventBus &bus, QWidget *parent) : QWidget(parent), scriptInfo_(scriptInfo), bus_(bus)
{
    setObjectName("ScriptCard");

    setMinimumWidth(220);
    setMinimumHeight(150);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setAttribute(Qt::WA_StyledBackground);

    registerListeners();

    std::string backgroundColor = scriptInfo.value("backgroundColor", "grey");
    std::string name = scriptInfo.value("name", "");

    std::string sheet = "background-color: " + backgroundColor + ";";

    setStyleSheet(QString::fromStdString(sheet));

    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(3, 3, 3, 3);
    layout_->setSpacing(8);

    QHBoxLayout *buttonsLayout_ = new QHBoxLayout();
    layout_->addLayout(buttonsLayout_);

    QString startIcon = IconManager::getIcon("start");
    startButton_ = new IconButton("", startIcon);
    startButton_->setFixedSize(65, 65);
    connect(startButton_, QPushButton::clicked, this, &ScriptCard::handleStartButtonClick);

    buttonsLayout_->addWidget(startButton_, 0, Qt::AlignLeft);

    nameLabel_ = new QLabel(QString::fromStdString(name), this);

    layout_->addWidget(nameLabel_, 0, Qt::AlignLeft | Qt::AlignBottom);
}

void ScriptCard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    emit requestDetailsPage(scriptInfo_);
}

void ScriptCard::registerListeners()
{
    bus_.subscribe("scripts.state", [this](const Message &message)
                   {
                       std::string name = scriptInfo_.value("name", "");
                       std::string type = message.data.value("state", "");

                       spdlog::debug("State event recieved {}", type);
                       if (message.scriptName != name || message.scriptName.empty())
                           return;

                       if (type == "scripts.started")
                           handleStart();
                       else if (type == "scripts.finished")
                           handleFinished();
                       else if (type == "scripts.error")
                           handleError(message); });
}

void ScriptCard::handleStartButtonClick()
{
    std::string id = scriptInfo_.value("name", "");
    if (!running)
        emit requestScriptStart(id, {});
    else
        emit requestScriptStop(id);
}

void ScriptCard::handleStart()
{
    spdlog::debug("Handling start event");
    running = true;

    QString stopIcon = IconManager::getIcon("stop");
    startButton_->setIcon(stopIcon);
}

void ScriptCard::handleFinished()
{
    running = false;

    QString startIcon = IconManager::getIcon("start");
    startButton_->setIcon(startIcon);
}

void ScriptCard::handleError(const Message &message)
{
    running = false;

    QString errorIcon = IconManager::getIcon("error");
    startButton_->setIcon(errorIcon);
    spdlog::error("Error occured in script {}", message.data.value<std::string>("error", ""));
}