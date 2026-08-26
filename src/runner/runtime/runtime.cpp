#include "runtime.hpp"

#include "../../core/process/process.hpp"
#include "../../core/paths.hpp"
#include "../../core/eventBus/eventBus.hpp"

#include <string>
#include <vector>

Runtime::Runtime(Paths &paths, EventBus &bus) : paths_(paths), bus_(bus)
{

}

void Runtime::run(const std::string &file, std::vector<std::string> args)
{

}


