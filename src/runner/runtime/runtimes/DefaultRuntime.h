#pragma once

#include "../Runtime.h"
#include "../../../core/process/ProcessHandler.h"
#include "../../../core/Paths.h"
#include "../../../core/eventBus/EventBus.h"

#include <vector>
#include <string>

class DefaultRuntime : public Runtime
{
public:
    using Runtime::Runtime;

protected:
    std::filesystem::path executable(const std::filesystem::path &script) const override;
    void prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const override;
    std::string name() const override;
};
