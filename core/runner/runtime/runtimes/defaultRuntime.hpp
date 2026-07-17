#ifndef DEF_RUNTIME_H
#define DEF_RUNTIME_H

#include <vector>
#include <string>

#include "../../../process/process.hpp"
#include "../runtime.hpp"

class DefaultRuntime : public Runtime
{
private:
    Process *process;

public:
    void run(const std::string &file, std::vector<std::string> args) override;
};

#endif