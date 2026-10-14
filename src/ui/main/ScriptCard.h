#ifndef SCRIPT_CARD_H
#define SCRIPT_CARD_H

#include "../reuseables/IconButton.h"

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
    ScriptCard(nlohmann::json scriptInfo, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void runRequested(std::string_view name);

private:
    nlohmann::json scriptInfo_;
    QLabel *nameLabel_;
    QVBoxLayout *layout_;
};

#endif