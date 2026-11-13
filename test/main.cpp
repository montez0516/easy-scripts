#include "../src/core/parsing/DotEnv.h"

#include <spdlog/spdlog.h>

#include <iostream>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    DotEnv env(".env");

    if (!env.load())
    {
        std::cerr << "Failed to load .env file" << std::endl;
    }
    env.print();

    std::string test = env.getenv("test");
    std::string test1 = env.getenv("test1");
    std::string test2 = env.getenv("test2");
    if (test != "1" || test1 != "2" || test2 != "3")
    {
        spdlog::error("test=\"{}\" test1=\"{}\" test2=\"{}\" {} {} {}", test, test1, test2, test == "1", test1 == "2", test2 == "3");
        throw std::runtime_error("DotEnv test failed");
    }

    return 0;
}