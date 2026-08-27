#ifndef RUNTIME_H
#define RUNTIME_H

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"
#include <filesystem>
#include <vector>
#include <memory>

namespace fs = std::filesystem;

class Runtime
{
private:
protected:
    std::vector<std::unique_ptr<Process>> runtimes_;

    Paths &paths_;
    EventBus &bus_;

    virtual std::filesystem::path executable(const std::filesystem::path &script) const;
    virtual void prepareArguments(const std::filesystem::path &script, std::vector<std::string> &args) const;

public:
    Runtime(Paths &paths, EventBus &bus);
    void run(const std::string &file, std::vector<std::string> args);
};

#endif