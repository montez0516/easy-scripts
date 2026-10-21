#ifndef SCRIPT_CARD_H
#define SCRIPT_CARD_H

#include "../reuseables/IconButton.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>

#include <string_view>

class ScriptCard : public QWidget
{
    Q_OBJECT
public:
    ScriptCard(nlohmann::json scriptInfo, EventBus &bus, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void handleStartButtonClick();

signals:
    void requestDetailsPage(const nlohmann::json &info);
    void requestScriptStart(const std::string &id, std::vector<std::string> args);
    void requestScriptStop(const std::string &id);

private:
    nlohmann::json scriptInfo_;
    QLabel *nameLabel_;
    QVBoxLayout *layout_;
    IconButton *startButton_;
    IconButton *optionsButton_;
    EventBus &bus_;
    bool running{false};

    void registerListeners();
    void handleStart();
    void handleFinished();
    void handleError(const Message &message);
};

#endif