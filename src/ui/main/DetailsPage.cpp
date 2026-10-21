#include "DetailsPage.h"
#include "../reuseables/IconButton.h"
#include "../ThemeManager.h"

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

    scriptName_ = new QLabel();
    scriptName_->setObjectName("ScriptName");
    topLayout_->addWidget(scriptName_);

    QString currentTheme = ThemeManager::currentTheme();

    QString closeIcon = QString::fromStdString((Paths::icons() / currentTheme.toStdString() / ("close-" + currentTheme + ".svg").toStdString()).string());
    IconButton *closeButton = new IconButton("", closeIcon);
    connect(closeButton, QPushButton::clicked, this, [this]()
            {
                bus_.unsubscribe("scripts.media", scriptMediaID_);
                emit closeDetailsPage(); });

    topLayout_->addWidget(closeButton, 0, Qt::AlignRight | Qt::AlignCenter);

    midLayout_ = new QHBoxLayout();
    layout_->addLayout(midLayout_, 8);

    fileEdit_ = new QTextEdit("File Contents");
    midLayout_->addWidget(fileEdit_);

    stdoutBrowser_ = new QTextBrowser();
    connect(this, DetailsPage::stdoutRecieved, this, DetailsPage::handleStdout);
    midLayout_->addWidget(stdoutBrowser_);

    bottomLayout_ = new QHBoxLayout();
    layout_->addLayout(bottomLayout_, 1);

    scriptMediaID_ = bus_.subscribe("scripts.media", [this](const Message &message)
                                    {
                                        std::string type = message.data.value("type", "");
                                        if(type == "message")
                                        {
                                            emit stdoutRecieved(QString::fromStdString(message.data.value("text", "")));
                                        } });
}

void DetailsPage::changeInfo(const nlohmann::json &info)
{
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