#include "scriptCard.hpp"
#include "../api/register.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QString>
#include <QLabel>

ScriptCard::ScriptCard(nlohmann::json scriptInfo, APIRegister &apiRegister, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister), scriptInfo_(scriptInfo)
{
    setObjectName("ScriptCard");

    spdlog::debug("ScriptCard(): script info {}", scriptInfo.dump());

    std::string backgroundColor = scriptInfo.value("backgroundColor", "grey");
    std::string name = scriptInfo.value("name", "");

    std::string sheet = "background-color: " + backgroundColor + ";";

    setStyleSheet(QString::fromStdString(sheet));

    layout_ = new QVBoxLayout(this);

    layout_->addStretch();

    nameLabel_ = new QLabel(QString::fromStdString(name), this);

    layout_->addWidget(nameLabel_, 0, Qt::AlignLeft | Qt::AlignBottom);
}

void ScriptCard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    auto method = apiRegister_.getMethod("scripts.run");

    if (!method)
        return;

    APIResponse response = method({.params = scriptInfo_.value("name", "")});
}