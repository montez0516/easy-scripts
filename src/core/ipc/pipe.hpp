#pragma once

#include <string>
#include <functional>

class Pipe
{
public:
    virtual std::string read() = 0;
    virtual void write(std::string_view message) = 0;
    virtual bool readyRead(std::function<void(std::string_view)> callback) = 0;
};