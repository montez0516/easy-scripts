#include "ScriptCard.h"
#include "IconButton.h"
#include "../../core/Paths.h"
#include "../ThemeManager.h"
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

    QString currentTheme = ThemeManager::currentTheme();

    QString startIcon = QString::fromStdString((Paths::icons() / currentTheme.toStdString() / ("start-" + currentTheme + ".svg").toStdString()).string());
    startButton_ = new IconButton("", startIcon);
    startButton_->setFixedSize(65, 65);
    connect(startButton_, QPushButton::clicked, this, &ScriptCard::handleStartButtonClick);

    buttonsLayout_->addWidget(startButton_, 0, Qt::AlignLeft);

    // QString optionsIcon = QString::fromStdString((Paths::icons() / currentTheme.toStdString() / ("options-" + currentTheme + ".svg").toStdString()).string());
    // optionsButton_ = new IconButton("", optionsIcon);
    // optionsButton_->setFixedSize(65, 65);
    // buttonsLayout_->addWidget(optionsButton_, 0, Qt::AlignRight);

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
                       if (message.id != name || message.id.empty())
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
    bus_.publish(running ? "scripts.pause" : "scripts.run", {.type = MessageType::Request, .id = scriptInfo_.value("name", ""), .data = {}});
}

void ScriptCard::handleStart()
{
    spdlog::debug("Handling start event");
    running = true;
    QString currentTheme = ThemeManager::currentTheme();

    QString pauseIcon = QString::fromStdString((Paths::icons() / currentTheme.toStdString() / ("pause-" + currentTheme + ".svg").toStdString()).string());
    startButton_->setIcon(pauseIcon);
}

void ScriptCard::handleFinished()
{
    running = false;
    QString currentTheme = ThemeManager::currentTheme();

    QString startIcon = QString::fromStdString((Paths::icons() / currentTheme.toStdString() / ("start-" + currentTheme + ".svg").toStdString()).string());
    startButton_->setIcon(startIcon);
}

void ScriptCard::handleError(const Message &message)
{
    running = false;

    QString errorIcon = QString::fromStdString((Paths::icons() / "error.svg").string());
    startButton_->setIcon(errorIcon);
    spdlog::error("Error occured in script {} {}", message.data.value("code", "unknown"), message.data.value<std::string>("error", ""));
}

void ScriptCard::handleMedia(const Message &message)
{
    spdlog::debug("Recieved media event {} {}", message.name, message.data.dump());
}