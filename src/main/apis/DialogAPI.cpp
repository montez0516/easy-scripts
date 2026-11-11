#include "DialogAPI.h"

#include "../../api/Register.h"

#include <spdlog/spdlog.h>

#include <QWidget>
#include <QInputDialog>
#include <QString>
#include <QStringList>

#include <string>
#include <vector>

DialogAPI::DialogAPI(APIRegister &apiRegister, QWidget *parent) : parent_(parent)
{
    if (parent == nullptr)
    {
        spdlog::error("DialogAPI parent is null");
    }

    spdlog::debug("DialogAPI parent is not null registering methods");

    apiRegister.registerMethod(
        "dialog.input",
        [this](const APIRequest &request)
        { return showInput(request); },
        APIThread::GUI);
    apiRegister.registerMethod(
        "dialog.select",
        [this](const APIRequest &request)
        { return showSelect(request); },
        APIThread::GUI);
}

APIResponse DialogAPI::showInput(const APIRequest &request)
{
    if (parent_ == nullptr)
    {
        return APIResponse::failure("Could not display input::input parent widget is null");
    }

    bool ok;
    QString input = QInputDialog::getText(parent_, "Enter Text:", "", QLineEdit::Normal, QString(), &ok);

    if (ok && !input.isEmpty())
        return APIResponse::success(input.toUtf8().toStdString());
    else
        return APIResponse::failure("User cancelled");
}

APIResponse DialogAPI::showSelect(const APIRequest &request)
{
    if (parent_ == nullptr)
    {
        return APIResponse::failure("Could not display input::list parent widget is null");
    }

    std::vector<std::string> list = request.params.value<std::vector<std::string>>("list", {});

    if (list.empty())
        return APIResponse::failure("No items found to select from in list");

    QStringList qList;
    qList.reserve(list.size());

    for (const std::string &str : list)
    {
        qList.append(QString::fromStdString(str));
    }

    bool ok;
    QString selected = QInputDialog::getItem(parent_, "Select Item:", "Choose an item:", qList, 0, false, &ok);

    if (ok && !selected.isEmpty())
        return APIResponse::success(selected.toUtf8().toStdString());
    else
        return APIResponse::failure("User cancelled");
}