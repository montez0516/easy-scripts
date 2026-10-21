#pragma once

#include "../../core/eventBus/EventBus.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>

#include <cstdint>

class DetailsPage : public QWidget
{
    Q_OBJECT
public:
    DetailsPage(EventBus &bus, QWidget *parent = nullptr);
    void changeInfo(const nlohmann::json &info);

public slots:
    void handleStdout(QString output);

signals:
    void closeDetailsPage();
    void stdoutRecieved(QString output);

private:
    EventBus &bus_;
    QVBoxLayout *layout_;
    QHBoxLayout *topLayout_;
    QHBoxLayout *midLayout_;
    QHBoxLayout *bottomLayout_;

    QLabel *scriptName_;
    QTextEdit *fileEdit_;
    QTextBrowser *stdoutBrowser_;

    std::uint64_t scriptMediaID_;

    QString getScriptFileContents(const QString &path);
};