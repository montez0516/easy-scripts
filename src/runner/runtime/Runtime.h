#pragma once

#include "../../core/process/ProcessHandler.h"
#include "../../core/Paths.h"
#include "../../core/eventBus/EventBus.h"
#include "../../core/communication/Message.h"

#include <filesystem>
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace fs = std::filesystem;

class Runtime
{
private:
protected:
    std::map<std::string, std::unique_ptr<Process>> runtimes_;

    Paths &paths_;
    EventBus &bus_;

    virtual std::filesystem::path executable(const std::filesystem::path &script) const;
    virtual void prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const;
    void registerListener();
    Process *getRuntime(const std::string &name);
    void shutdown();
    virtual std::string name() const;

public:
    Runtime(Paths &paths, EventBus &bus);
    ~Runtime();
    void run(const Message &);
};
