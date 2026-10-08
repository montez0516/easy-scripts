#ifndef SCRIPT_CARD_H
#define SCRIPT_CARD_H

#include "../api/register.hpp"
#include "../reuseables/iconButton.hpp"

#include <nlohmann/json.hpp>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>

class ScriptCard : public QWidget
{
public:
    ScriptCard(nlohmann::json scriptInfo, APIRegister &apiRegister, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QLabel *nameLabel_;
    QVBoxLayout *layout_;
    APIRegister &apiRegister_;

    nlohmann::json scriptInfo_;
};

#endif