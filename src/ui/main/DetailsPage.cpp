#include "DetailsPage.h"
#include "../reuseables/IconButton.h"
#include "../utils/ThemeManager.h"
#include "../utils/IconManager.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <Qt>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>
#include <QFile>

DetailsPage::DetailsPage(EventBus &bus, QWidget *parent) : QWidget(parent), bus_(bus)
{
    setObjectName("DetailsPage");

    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setSpacing(8);

    topLayout_ = new QHBoxLayout();
    layout_->addLayout(topLayout_, 1);

    // QString startIcon = IconManager::getIcon("start");
    QString startIcon = IconManager::getIcon("start");
    startButton_ = new IconButton("", startIcon);
    connect(startButton_, &QPushButton::clicked, this, &DetailsPage::handleStartButtonClick);
    topLayout_->addWidget(startButton_, 1);

    scriptName_ = new QLabel();
    scriptName_->setObjectName("ScriptName");
    topLayout_->addWidget(scriptName_, 8);

    QString closeIcon = IconManager::getIcon("close");
    IconButton *closeButton = new IconButton("", closeIcon);
    connect(closeButton, QPushButton::clicked, this, [this]()
            {
                bus_.unsubscribe("scripts.media", scriptMediaID_);
                emit closeDetailsPage(); });

    topLayout_->addWidget(closeButton, 1);

    midLayout_ = new QHBoxLayout();
    layout_->addLayout(midLayout_, 8);

    fileEdit_ = new QTextEdit("File Contents");
    midLayout_->addWidget(fileEdit_);

    stdoutBrowser_ = new QTextBrowser();
    connect(this, DetailsPage::stdoutRecieved, this, DetailsPage::handleStdout);
    midLayout_->addWidget(stdoutBrowser_);

    bottomLayout_ = new QHBoxLayout();
    layout_->addLayout(bottomLayout_, 1);
}

void DetailsPage::changeInfo(const nlohmann::json &info)
{
    scriptInfo_ = info;
    scriptName_->setText(QString::fromStdString(info.value<std::string>("name", "")));

    fileEdit_->setText(getScriptFileContents(QString::fromStdString(info.value<std::string>("file", ""))));
}

QString DetailsPage::getScriptFileContents(const QString &path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString contents = file.readAll();

    return contents;
}

void DetailsPage::handleStdout(QString output)
{
    QTextCursor cursor = stdoutBrowser_->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(output);
}

void DetailsPage::activate()
{
    if (!scriptMediaID_)
        scriptMediaID_ = bus_.subscribe("scripts.media", [this](const Message &message)
                                        {
            std::string type = message.data.value<std::string>("type", "");

            if(type == "message")
            {
                emit stdoutRecieved(QString::fromStdString(message.data.value<std::string>("text", "")));
            } });
    if (!scriptStateID_)
        scriptStateID_ = bus_.subscribe("scripts.state", [this](const Message &message)
                                        {
                        std::string name = scriptInfo_.value("name", "");
                        std::string type = message.data.value("state", "");

                        if (message.scriptName != name || message.scriptName.empty())
                            return;
                            spdlog::debug("State event recieved in DetailsPage");
                        if (type == "scripts.started")
                            handleStart();
                        else if (type == "scripts.finished")
                            handleFinished();
                        else if (type == "scripts.error")
                            handleError(message); });
}

void DetailsPage::deactivate()
{
    if (scriptMediaID_)
    {
        bus_.unsubscribe("scripts.media", scriptMediaID_);
        scriptMediaID_ = 0;
    }
    if (scriptStateID_)
    {
        bus_.unsubscribe("scripts.media", scriptStateID_);
        scriptStateID_ = 0;
    }
}

void DetailsPage::handleStartButtonClick()
{
    std::string id = scriptInfo_.value("name", "");
    if (!running_)
        emit requestScriptStart(id, {});
    else
        emit requestScriptStop(id);
}

void DetailsPage::handleStart()
{
    running_ = true;
    QString currentTheme = ThemeManager::currentTheme();

    QString stopIcon = IconManager::getIcon("stop");
    startButton_->setIcon(stopIcon);
    fileEdit_->hide();
}

void DetailsPage::handleFinished()
{
    running_ = false;

    QString startIcon = IconManager::getIcon("start");
    startButton_->setIcon(startIcon);
    fileEdit_->show();
}

void DetailsPage::handleError(const Message &message)
{
    running_ = false;

    QString errorIcon = IconManager::getIcon("error");
    startButton_->setIcon(errorIcon);
    spdlog::error("Error occured in script {} {}", message.data.value("code", "unknown"), message.data.value<std::string>("error", ""));
}