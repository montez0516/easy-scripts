#pragma once

#include <string>
#include <vector>

class MessageFramer
{
public:
    std::vector<std::string> push(std::string_view message);

private:
    std::string buffer_;
};