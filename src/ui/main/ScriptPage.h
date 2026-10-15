#pragma once

#include "ScriptCard.h"
#include "../UIContext.h"
#include "../reuseables/FlowLayout.h"

#include <QWidget>
#include <QVBoxLayout>

#include <string_view>
#include <unordered_map>

class ScriptPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptPage(UIContext &uiContext, QWidget *parent = nullptr);

private:
    UIContext &uiContext_;
    FlowLayout *layout_;
    std::unordered_map<std::string, ScriptCard *> scriptLookup;
};