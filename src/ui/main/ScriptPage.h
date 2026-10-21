#pragma once

#include "ScriptCard.h"
#include "../UIContext.h"
#include "../reuseables/FlowLayout.h"
#include "DetailsPage.h"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QVBoxLayout>
#include <QStackedLayout>

#include <string_view>
#include <unordered_map>
#include <cstdint>

class ScriptPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptPage(UIContext &uiContext, QWidget *parent = nullptr);

public slots:
    void displayDetailsPage(const nlohmann::json &info);
    void sendStartRequest(const std::string &id, std::vector<std::string> args);
    void sendStopRequest(const std::string &id);

private:
    UIContext &uiContext_;
    QStackedLayout *layout_;
    QWidget *scriptPage_;
    FlowLayout *scriptLayout_;
    DetailsPage *detailsPage_;
    std::unordered_map<std::string, ScriptCard *> scriptLookup_;
};