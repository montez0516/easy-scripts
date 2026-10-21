#pragma once

#include "../../core/communication/Message.h"
#include "../../core/eventBus/EventBus.h"
#include "../reuseables/IconButton.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>

#include <cstdint>
#include <string>
#include <vector>

class DetailsPage : public QWidget
{
    Q_OBJECT
public:
    DetailsPage(EventBus &bus, QWidget *parent = nullptr);
    void changeInfo(const nlohmann::json &info);
    void activate();
    void deactivate();

public slots:
    void handleStdout(QString output);

signals:
    void closeDetailsPage();
    void stdoutRecieved(QString output);
    void requestScriptStart(const std::string &id, std::vector<std::string> args);
    void requestScriptStop(const std::string &id);

private:
    EventBus &bus_;
    nlohmann::json scriptInfo_;
    QVBoxLayout *layout_;
    QHBoxLayout *topLayout_;
    QHBoxLayout *midLayout_;
    QHBoxLayout *bottomLayout_;

    QLabel *scriptName_;
    QTextEdit *fileEdit_;
    QTextBrowser *stdoutBrowser_;
    IconButton *startButton_;

    std::uint64_t scriptMediaID_{0};
    std::uint64_t scriptStateID_{0};
    bool running_;

    QString getScriptFileContents(const QString &path);
    void handleStartButtonClick();
    void handleStart();
    void handleFinished();
    void handleError(const Message &message);
};