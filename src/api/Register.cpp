#include "Register.h"

#include <QApplication>
#include <QMetaObject>
#include <QThread>

#include <string>
#include <string_view>
#include <functional>

bool APIRegister::registerMethod(std::string name, APIMethod method, APIThread thread)
{
    if (!method)
        return false;

    RegisteredMethod registeredMethod{.callback = std::move(method), .thread = thread};

    return methods_.emplace(std::move(name), std::move(registeredMethod)).second;
}

bool APIRegister::unregisterMethod(std::string_view name)
{
    return methods_.erase(std::string(name)) != 0;
}

bool APIRegister::hasMethod(std::string_view name) const
{
    return methods_.contains(std::string(name));
}

APIResponse APIRegister::call(std::string_view name, const APIRequest &request) const
{
    auto it = methods_.find(std::string(name));

    if (it == methods_.end())
    {
        return APIResponse::failure("Unknown API method: " + std::string(name));
    }

    try
    {
        auto method = it->second;

        if (method.thread == APIThread::Caller)
            return method.callback(request);
        if (QThread::currentThread() == qApp->thread())
            return method.callback(request);

        APIResponse response;

        bool success = QMetaObject::invokeMethod(
            qApp,
            [&]()
            { response = method.callback(request); },
            Qt::BlockingQueuedConnection);

        if (!success)
            return APIResponse::failure("Failed to dispatch API method");

        return response;
    }

    catch (const std::exception &e)
    {
        return APIResponse::failure(e.what());
    }
    catch (...)
    {
        return APIResponse::failure("Unknown API error");
    }
}