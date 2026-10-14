#pragma once

#include "ScriptCard.h"
#include "../UIContext.h"
#include "../reuseables/FlowLayout.h"

#include <QWidget>
#include <QVBoxLayout>

#include <string_view>

class ScriptPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptPage(UIContext &uiContext, QWidget *parent = nullptr);

public slots:
    void sendRunRequest(std::string_view name);

private:
    UIContext &uiContext_;
    FlowLayout *layout_;
};