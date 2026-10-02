#ifndef RUNTIME_H
#define RUNTIME_H

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"
#include "../../core/communication/message.hpp"

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

public:
    Runtime(Paths &paths, EventBus &bus);
    void run(const Message &);
};

#endif